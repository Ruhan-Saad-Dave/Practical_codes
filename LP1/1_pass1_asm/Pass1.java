import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Implements Pass 1 of a simple two-pass assembler for a SIC-like machine.
 * This pass reads an assembly source file, builds a symbol table (SYMTAB),
 * a literal table (LITTAB), a pool table (POOLTAB), and generates an
 * intermediate file for Pass 2.
 */
public class Pass1 {

    // --- Data Structures ---
    private static final Map<String, Integer> symtab = new LinkedHashMap<>();
    private static final Map<String, String> optab = new HashMap<>();
    private static final Map<String, LiteralEntry> littab = new LinkedHashMap<>();
    private static final List<Integer> pooltab = new ArrayList<>();

    private static int locationCounter = 0;
    private static int startingAddress = 0;

    /**
     * Represents an entry in the Literal Table (LITTAB).
     */
    static class LiteralEntry {
        String literal;
        String value;
        int length;
        int address = -1; // -1 indicates address is not yet assigned

        LiteralEntry(String literal) {
            this.literal = literal;
            String content = literal.substring(3, literal.length() - 1); // Content inside quotes

            if (literal.toUpperCase().startsWith("=C'")) {
                this.length = content.length();
                StringBuilder hexValue = new StringBuilder();
                for (char c : content.toCharArray()) {
                    hexValue.append(Integer.toHexString(c).toUpperCase());
                }
                this.value = hexValue.toString();
            } else if (literal.toUpperCase().startsWith("=X'")) {
                this.length = (content.length() + 1) / 2; // Each pair of hex digits is 1 byte
                this.value = content;
            }
        }
    }

    /**
     * Pre-populates the Opcode Table (OPTAB).
     */
    private static void initializeOptab() {
        optab.put("ADD", "18");
        optab.put("AND", "40");
        optab.put("COMP", "28");
        optab.put("DIV", "24");
        optab.put("J", "3C");
        optab.put("JEQ", "30");
        optab.put("JGT", "34");
        optab.put("JLT", "38");
        optab.put("JSUB", "48");
        optab.put("LDA", "00");
        optab.put("LDCH", "50");
        optab.put("LDL", "08");
        optab.put("LDX", "04");
        optab.put("MUL", "20");
        optab.put("OR", "44");
        optab.put("RD", "D8");
        optab.put("RSUB", "4C");
        optab.put("STA", "0C");
        optab.put("STCH", "54");
        optab.put("STL", "14");
        optab.put("STX", "10");
        optab.put("SUB", "1C");
        optab.put("TD", "E0");
        optab.put("TIX", "2C");
        optab.put("WD", "DC");
    }

    public static void main(String[] args) {
        initializeOptab();
        try {
            assemble("input.txt");
            System.out.println("Pass 1 assembly complete. Check intermediate.txt, symtab.txt, littab.txt, and pooltab.txt.");
        } catch (IOException e) {
            System.err.println("Error during assembly process: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Performs the Pass 1 assembly process.
     * @param inputFile The path to the assembly source code file.
     * @throws IOException If there's an error reading or writing files.
     */
    public static void assemble(String inputFile) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile));
             FileWriter intermediateWriter = new FileWriter("intermediate.txt");
             FileWriter symtabWriter = new FileWriter("symtab.txt");
             FileWriter littabWriter = new FileWriter("littab.txt");
             FileWriter pooltabWriter = new FileWriter("pooltab.txt")) {

            pooltab.add(0); // Initialize POOLTAB for the first literal pool at index 0
            String line;
            boolean firstLine = true;

            while ((line = reader.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty() || line.startsWith(".")) {
                    continue;
                }

                // --- Parsing the line ---
                String label = null, opcode = null, operand = null;
                String[] tokens = line.split("\\s+", 3);
                String firstToken = tokens[0];

                // Check if the first token is an opcode or directive
                boolean isFirstTokenOpcode = optab.containsKey(firstToken.replace("+", "").toUpperCase()) ||
                   "WORD,RESW,RESB,BYTE,START,END,LTORG".contains(firstToken.toUpperCase());

                if (isFirstTokenOpcode) {
                    opcode = tokens[0];
                    if (tokens.length > 1) operand = tokens[1];
                } else {
                    label = tokens[0];
                    opcode = (tokens.length > 1) ? tokens[1] : null;
                    if (tokens.length > 2) operand = tokens[2];
                }

                // --- Handle START directive ---
                if (firstLine) {
                    if ("START".equalsIgnoreCase(opcode)) {
                        startingAddress = Integer.parseInt(operand, 16);
                        locationCounter = startingAddress;
                        intermediateWriter.write(String.format("%-8s %-8s %-8s %s\n", Integer.toHexString(locationCounter).toUpperCase(), label, opcode, operand));
                    } else {
                        locationCounter = 0;
                    }
                    firstLine = false;
                    continue;
                }

                // --- Handle LTORG and END directives (process literal pool) ---
                if ("LTORG".equalsIgnoreCase(opcode) || "END".equalsIgnoreCase(opcode)) {
                    intermediateWriter.write(String.format("%-8s %-8s %-8s %s\n", "", label != null ? label : "", opcode, operand != null ? operand : ""));
                    processLiteralPool(intermediateWriter);
                    if ("END".equalsIgnoreCase(opcode)) break;
                    continue;
                }

                // --- Write current line to intermediate file before updating LC ---
                intermediateWriter.write(String.format("%-8s %-8s %-8s %s\n", Integer.toHexString(locationCounter).toUpperCase(), label != null ? label : "", opcode, operand != null ? operand : ""));

                // --- SYMTAB processing ---
                if (label != null && !label.isEmpty()) {
                    if (symtab.containsKey(label)) {
                        System.err.println("ERROR: Duplicate symbol found -> " + label);
                    } else {
                        symtab.put(label, locationCounter);
                    }
                }
                
                // --- LITTAB processing ---
                if (operand != null && operand.startsWith("=")) {
                    if (!littab.containsKey(operand)) {
                        littab.put(operand, new LiteralEntry(operand));
                    }
                }

                // --- Location Counter processing ---
                boolean isFormat4 = opcode != null && opcode.startsWith("+");
                String cleanOpcode = isFormat4 ? opcode.substring(1) : opcode;

                if (optab.containsKey(cleanOpcode.toUpperCase())) {
                    locationCounter += isFormat4 ? 4 : 3;
                } else if ("WORD".equalsIgnoreCase(opcode)) {
                    locationCounter += 3;
                } else if ("RESW".equalsIgnoreCase(opcode)) {
                    locationCounter += 3 * Integer.parseInt(operand);
                } else if ("RESB".equalsIgnoreCase(opcode)) {
                    locationCounter += Integer.parseInt(operand);
                } else if ("BYTE".equalsIgnoreCase(opcode)) {
                    if (operand.toUpperCase().startsWith("C'")) {
                        locationCounter += operand.length() - 3;
                    } else if (operand.toUpperCase().startsWith("X'")) {
                        locationCounter += (operand.length() - 3) / 2;
                    }
                } else {
                     System.err.println("ERROR: Invalid opcode found -> " + opcode);
                }
            }

            // --- Write out the generated tables ---
            for (Map.Entry<String, Integer> entry : symtab.entrySet()) {
                symtabWriter.write(String.format("%-10s %s\n", entry.getKey(), Integer.toHexString(entry.getValue()).toUpperCase()));
            }
            for (Map.Entry<String, LiteralEntry> entry : littab.entrySet()) {
                LiteralEntry lit = entry.getValue();
                littabWriter.write(String.format("%-8s %-8s %-4d %s\n", lit.literal, lit.value, lit.length, Integer.toHexString(lit.address).toUpperCase()));
            }
            for (int i = 0; i < pooltab.size() - 1; i++) {
                pooltabWriter.write(String.format("#%d\n", pooltab.get(i)));
            }
        }
    }

    /**
     * Processes the current literal pool by assigning addresses to all literals
     * collected since the last pool was processed.
     * @param writer The FileWriter for the intermediate file.
     * @throws IOException If there's an error writing to the file.
     */
    private static void processLiteralPool(FileWriter writer) throws IOException {
        int poolStartIndex = pooltab.get(pooltab.size() - 1);
        
        // Convert LITTAB map values to a list to access by index
        List<LiteralEntry> literalList = new ArrayList<>(littab.values());

        for (int i = poolStartIndex; i < literalList.size(); i++) {
            LiteralEntry entry = literalList.get(i);
            if (entry.address == -1) { // If address is not yet assigned
                entry.address = locationCounter;
                writer.write(String.format("%-8s %-8s %-8s\n",
                    Integer.toHexString(locationCounter).toUpperCase(), "*", entry.literal));
                locationCounter += entry.length;
            }
        }
        // Mark the start of the next literal pool
        pooltab.add(littab.size());
    }
}

