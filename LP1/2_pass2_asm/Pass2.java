import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Implements Pass-II of a two-pass assembler for the hypothetical machine
 * described in the practical assignment.
 *
 * Reads: intermediate_code.txt, symtab.txt, littab.txt, pooltab.txt
 * Writes: target_code.txt
 */
public class Pass2 {

    // Data structures to hold tables read from files
    private static final List<Integer> symAddresses = new ArrayList<>();
    private static final List<LiteralData> litTable = new ArrayList<>();
    private static final List<Integer> pooltab = new ArrayList<>();

    /**
     * Helper class to store data from LITTAB.
     */
    static class LiteralData {
        String literal; // e.g., "='5'"
        int address;    // e.g., 208

        LiteralData(String literal, int address) {
            this.literal = literal;
            this.address = address;
        }
    }

    /**
     * Main entry point.
     */
    public static void main(String[] args) {
        try {
            loadTables();
            generateTargetCode();
            System.out.println("Pass 2 complete. Check target_code.txt.");
        } catch (IOException e) {
            System.err.println("Error during Pass 2: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Loads SYMTAB, LITTAB, and POOLTAB from their respective files
     * into memory (Lists).
     */
    private static void loadTables() throws IOException {
        // Load Symbol Table (symtab.txt)
        try (BufferedReader reader = new BufferedReader(new FileReader("symtab.txt"))) {
            String line;
            reader.readLine(); // Skip header
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t");
                // The IC uses index (S, 00), so we just add the address
                // to a list. The list's index (0) will match (S, 00).
                symAddresses.add(Integer.parseInt(parts[1]));
            }
        }

        // Load Literal Table (littab.txt)
        try (BufferedReader reader = new BufferedReader(new FileReader("littab.txt"))) {
            String line;
            reader.readLine(); // Skip header
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t");
                // The IC uses index (L, 00), so we add a new LiteralData
                // object to a list. The list's index (0) will match (L, 00).
                litTable.add(new LiteralData(parts[1], Integer.parseInt(parts[2])));
            }
        }

        // Load Pool Table (pooltab.txt)
        try (BufferedReader reader = new BufferedReader(new FileReader("pooltab.txt"))) {
            String line;
            reader.readLine(); // Skip header
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t"); // <-- FIX 1: Added this line
                // The pool table just stores the starting index for each pool.
                pooltab.add(Integer.parseInt(parts[1]));
            }
        }
    }

    /**
     * Reads intermediate_code.txt and generates the final target_code.txt.
     */
    private static void generateTargetCode() throws IOException {
        try (BufferedReader icReader = new BufferedReader(new FileReader("intermediate_code.txt"));
             FileWriter targetWriter = new FileWriter("target_code.txt")) {

            int locationCounter = 0;
            int poolPtr = 0; // To keep track of which literal pool we are in

            // This regex is designed to parse all formats in your sample IC:
            // e.g., (AD, 00) (C, 200)
            // e.g., (IS, 09) (S, 00)
            // e.g., (IS, 04) (0) (L, 00)
            // e.g., (AD, 04)
            String icRegex = "^\\((\\w+), (\\d+)\\)\\s*(?:\\((\\d+)\\))?\\s*(?:\\((C|S|L), (\\d+)\\))?";
            Pattern pattern = Pattern.compile(icRegex);

            String line;
            while ((line = icReader.readLine()) != null) {
                Matcher m = pattern.matcher(line);
                if (!m.find()) continue;

                String type = m.group(1);  // e.g., "AD", "IS", "DL"
                String code = m.group(2);  // e.g., "00", "09", "01"
                String reg = m.group(3);   // e.g., "0" or null
                String opType = m.group(4); // e.g., "C", "S", "L" or null
                String opVal = m.group(5);  // e.g., "200", "00", "01" or null

                if ("AD".equals(type)) {
                    // --- Assembler Directive ---
                    if ("00".equals(code)) { // START
                        locationCounter = Integer.parseInt(opVal);
                    } else if ("01".equals(code) || "04".equals(code)) { // END or LTORG
                        // Process the current literal pool
                        locationCounter = processLiteralPool(targetWriter, locationCounter, poolPtr);
                        poolPtr++; // Move to the next pool
                        if ("01".equals(code)) break; // END
                    }
                } else if ("DL".equals(type)) {
                    // --- Declarative Statement ---
                    // e.g., (DL, 01) (C, 01) for "DS 1"
                    // As per sample, this just reserves space. We output a blank line
                    // and increment the LC.
                    int size = Integer.parseInt(opVal);
                    for(int i=0; i<size; i++) {
                        targetWriter.write(String.format("%d)\n", locationCounter));
                        locationCounter++;
                    }
                } else if ("IS".equals(type)) {
                    // --- Imperative Statement ---
                    String opCodeStr = code;
                    String regStr = (reg != null) ? reg : "0"; // Default register 0
                    int operandAddr = 0;

                    if (opType != null) {
                        int index = Integer.parseInt(opVal);
                        if ("S".equals(opType)) { // Symbol
                            operandAddr = symAddresses.get(index);
                        } else if ("L".equals(opType)) { // Literal
                            operandAddr = litTable.get(index).address;
                        }
                    }
                    // Format: 200) + 09 0 216
                    targetWriter.write(String.format("%d) + %s %s %03d\n",
                            locationCounter, opCodeStr, regStr, operandAddr));
                    locationCounter++;
                }
            }
        }
    }

    /**
     * Helper method to process literals for LTORG or END.
     *
     * @param writer        The file writer for target_code.txt
     * @param currentLc     The current location counter
     * @param poolPtr       The index of the current pool in pooltab
     * @return The *new* location counter after processing literals
     */
    private static int processLiteralPool(FileWriter writer, int currentLc, int poolPtr) throws IOException {
        int lc = currentLc;
        
        // Get the start index of the current pool
        int poolStart = pooltab.get(poolPtr); // <-- FIX 2: Changed poolAptr to poolPtr
        
        // Get the end index (start of next pool, or end of table)
        int poolEnd = (poolPtr + 1 < pooltab.size()) ? pooltab.get(poolPtr + 1) : litTable.size();

        for (int i = poolStart; i < poolEnd; i++) {
            LiteralData lit = litTable.get(i);
            
            // Extract value, e.g., "='5'" -> "5"
            String valueStr = lit.literal.replaceAll("='|'", "");
            int value = Integer.parseInt(valueStr);

            // Format: 208) + 00 0 005
            writer.write(String.format("%d) + 00 0 %03d\n", lc, value));
            lc++;
        }
        return lc; // Return the updated LC
    }
}