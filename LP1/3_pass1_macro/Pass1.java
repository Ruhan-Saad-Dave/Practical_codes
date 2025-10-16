import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

/**
 * Represents an entry in the Macro Name Table (MNT).
 */
class MntEntry {
    String name;
    int pp; // Number of positional parameters
    int kp; // Number of keyword parameters
    int mdtp; // Pointer to the Macro Definition Table
    int kpdtp; // Pointer to the Keyword Parameter Default Value Table

    public MntEntry(String name, int pp, int kp, int mdtp, int kpdtp) {
        this.name = name;
        this.pp = pp;
        this.kp = kp;
        this.mdtp = mdtp;
        this.kpdtp = kpdtp;
    }

    @Override
    public String toString() {
        return String.format("%-10s\t%d\t%d\t%d\t%d", name, pp, kp, mdtp, kpdtp);
    }
}

public class Pass1 {

    // Data Structures for Pass 1
    private static List<MntEntry> mnt = new ArrayList<>();
    private static List<String> mdt = new ArrayList<>();
    private static List<String> kpdt = new ArrayList<>();
    private static Map<String, Integer> pntab = new HashMap<>();

    // Counters
    private static int mntc = 0;
    private static int mdtc = 0;
    private static int kpdtc = 0;

    public static void main(String[] args) {
        try {
            System.out.println("Starting Pass 1 of the Macro Processor...");
            process("input.txt");
            System.out.println("Pass 1 completed successfully.");
            
            // Write the generated tables to output files
            writeTables();
            System.out.println("Output tables have been written to files (mnt.txt, mdt.txt, kpdt.txt, pntab.txt).");
            System.out.println("Intermediate code has been written to intermediate_code.txt.");

        } catch (IOException e) {
            System.err.println("An error occurred during file processing: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Processes the input assembly language file.
     * @param inputFileName The name of the file to process.
     * @throws IOException If an I/O error occurs.
     */
    private static void process(String inputFileName) throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(inputFileName));
             BufferedWriter bw = new BufferedWriter(new FileWriter("intermediate_code.txt"))) {
            
            String line;
            boolean isMacroDefinition = false;

            while ((line = br.readLine()) != null) {
                line = line.trim();
                StringTokenizer st = new StringTokenizer(line, " \t,");
                String firstToken = st.hasMoreTokens() ? st.nextToken() : "";

                if (firstToken.equalsIgnoreCase("MACRO")) {
                    isMacroDefinition = true;
                    // The next line is the macro prototype
                    String prototypeLine = br.readLine().trim();
                    processMacroPrototype(prototypeLine);
                } else if (firstToken.equalsIgnoreCase("MEND")) {
                    isMacroDefinition = false;
                    mdt.add(mdtc++, "MEND");
                } else if (isMacroDefinition) {
                    // This is a line inside the macro definition
                    mdt.add(mdtc++, substituteParameters(line));
                } else {
                    // This is regular assembly code
                    bw.write(line);
                    bw.newLine();
                }
            }
        }
    }

    /**
     * Processes the macro prototype line to populate MNT, PNTAB, and KPDTAB.
     * @param prototypeLine The line containing the macro name and parameters.
     */
    private static void processMacroPrototype(String prototypeLine) {
        StringTokenizer st = new StringTokenizer(prototypeLine, " \t,");
        String macroName = st.nextToken();
        
        pntab.clear(); // Clear for the new macro definition
        int ppCount = 0;
        int kpCount = 0;
        int paramIndex = 1;

        while (st.hasMoreTokens()) {
            String param = st.nextToken();
            if (param.contains("=")) { // Keyword parameter
                kpCount++;
                String[] parts = param.split("=");
                String paramName = parts[0].trim();
                String defaultValue = (parts.length > 1) ? parts[1].trim() : "";
                
                pntab.put(paramName, paramIndex++);
                kpdt.add(kpdtc++, defaultValue);
            } else { // Positional parameter
                ppCount++;
                pntab.put(param.trim(), paramIndex++);
            }
        }
        
        mnt.add(new MntEntry(macroName, ppCount, kpCount, mdtc, (kpCount > 0 ? (kpdtc - kpCount) : 0)));
        mntc++;

        // Save PNTAB state for output
        savePntab(macroName);
    }
    
    /**
     * Substitutes formal parameters with their positional notation (e.g., &ARG -> (P,1)).
     * @param line The line from the macro definition body.
     * @return The line with substituted parameters.
     */
    private static String substituteParameters(String line) {
        StringTokenizer st = new StringTokenizer(line, " \t,");
        StringBuilder newLine = new StringBuilder(st.nextToken()); // Keep the instruction
        
        while (st.hasMoreTokens()) {
            String token = st.nextToken();
            if (pntab.containsKey(token)) {
                newLine.append(" (P,").append(pntab.get(token)).append(")");
            } else {
                newLine.append(" ").append(token);
            }
        }
        return newLine.toString();
    }
    
    /**
     * Writes the content of all tables to their respective output files.
     * @throws IOException If an I/O error occurs.
     */
    private static void writeTables() throws IOException {
        // Write MNT
        try (BufferedWriter bw = new BufferedWriter(new FileWriter("mnt.txt"))) {
            bw.write("Macro Name\t#PP\t#KP\tMDTP\tKPDTP\n");
            bw.write("----------------------------------------------\n");
            for (MntEntry entry : mnt) {
                bw.write(entry.toString());
                bw.newLine();
            }
        }
        
        // Write MDT
        try (BufferedWriter bw = new BufferedWriter(new FileWriter("mdt.txt"))) {
            bw.write("Index\tInstruction\n");
            bw.write("-------------------------\n");
            for (int i = 0; i < mdt.size(); i++) {
                bw.write(String.format("%-5d\t%s\n", i, mdt.get(i)));
            }
        }

        // Write KPDT
        try (BufferedWriter bw = new BufferedWriter(new FileWriter("kpdt.txt"))) {
            bw.write("Index\tDefault Value\n");
            bw.write("-------------------------\n");
            for (int i = 0; i < kpdt.size(); i++) {
                bw.write(String.format("%-5d\t%s\n", i, kpdt.get(i)));
            }
        }
    }

    /**
     * Saves the current state of PNTAB for a given macro to a file.
     * Appends to the file for each macro definition found.
     * @param macroName The name of the macro.
     */
    private static void savePntab(String macroName) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter("pntab.txt", true))) {
            bw.write("---- PNTAB for Macro: " + macroName + " ----\n");
            bw.write("Parameter Name\tIndex\n");
            bw.write("-------------------------\n");
            for (Map.Entry<String, Integer> entry : pntab.entrySet()) {
                bw.write(String.format("%-15s\t%d\n", entry.getKey(), entry.getValue()));
            }
            bw.write("\n");
        } catch (IOException e) {
            System.err.println("Error writing PNTAB to file: " + e.getMessage());
        }
    }
}
