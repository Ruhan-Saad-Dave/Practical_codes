import java.io.*;
import java.nio.file.*;
import java.util.*;

public class pass2 {

    static class MntEntry {
        String name;
        int mdtStart;
        List<String> formals; // include & in names, e.g. &ARG1

        MntEntry(String name, int mdtStart, List<String> formals) {
            this.name = name;
            this.mdtStart = mdtStart;
            this.formals = formals;
        }
    }

    public static void main(String[] args) {
        String mntFile = "MNT.txt";
        String mdtFile = "MDT.txt";
        String sourceFile = "source.asm";
        String outputFile = "expanded.asm";

        try {
            Map<String, MntEntry> mnt = loadMNT(mntFile);
            Map<Integer, String> mdt = loadMDT(mdtFile);

            expandSource(sourceFile, outputFile, mnt, mdt);
            System.out.println("Expansion completed. Output written to: " + outputFile);
        } catch (IOException e) {
            System.err.println("I/O error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // Load MNT: each line -> macroName|mdtStart|&p1,&p2,...
    private static Map<String, MntEntry> loadMNT(String path) throws IOException {
        Map<String, MntEntry> mnt = new HashMap<>();
        List<String> lines = Files.readAllLines(Paths.get(path));
        int lineNo = 0;
        for (String l : lines) {
            lineNo++;
            l = l.trim();
            if (l.isEmpty()) continue;
            String[] parts = l.split("\\|");
            if (parts.length < 2) {
                throw new IOException("Invalid MNT format at line " + lineNo + ": " + l);
            }
            String name = parts[0].trim();
            int mdtStart = Integer.parseInt(parts[1].trim());
            List<String> formals = new ArrayList<>();
            if (parts.length >= 3 && !parts[2].trim().isEmpty()) {
                String[] params = parts[2].split(",");
                for (String p : params) {
                    formals.add(p.trim());
                }
            }
            mnt.put(name, new MntEntry(name, mdtStart, formals));
        }
        return mnt;
    }

    // Load MDT: each line -> index:text
    private static Map<Integer, String> loadMDT(String path) throws IOException {
        Map<Integer, String> mdt = new TreeMap<>();
        List<String> lines = Files.readAllLines(Paths.get(path));
        int lineNo = 0;
        for (String l : lines) {
            lineNo++;
            l = l.trim();
            if (l.isEmpty()) continue;
            int colonIndex = l.indexOf(':');
            if (colonIndex < 0) {
                throw new IOException("Invalid MDT format at line " + lineNo + ": " + l);
            }
            String idxStr = l.substring(0, colonIndex).trim();
            String text = l.substring(colonIndex + 1).trim();
            int idx = Integer.parseInt(idxStr);
            mdt.put(idx, text);
        }
        return mdt;
    }

    // Expand source file
    private static void expandSource(String srcPath, String outPath,
                                     Map<String, MntEntry> mnt, Map<Integer, String> mdt) throws IOException {
        List<String> lines = Files.readAllLines(Paths.get(srcPath));
        BufferedWriter bw = Files.newBufferedWriter(Paths.get(outPath));

        for (String raw : lines) {
            String line = raw.trim();
            if (line.isEmpty()) {
                bw.write("");
                bw.newLine();
                continue;
            }

            // split first token (opcode / possible macro name) and rest
            String[] tokens = line.split("\\s+", 2);
            String first = tokens[0].trim();

            if (mnt.containsKey(first)) {
                // Macro invocation
                MntEntry entry = mnt.get(first);

                // Parse actual parameters (if any)
                String actualPart = (tokens.length > 1) ? tokens[1].trim() : "";
                List<String> actuals = new ArrayList<>();
                if (!actualPart.isEmpty()) {
                    // split on commas but also trim spaces
                    for (String a : actualPart.split(",")) actuals.add(a.trim());
                }

                // Build ALA (formal -> actual). Positional mapping.
                Map<String, String> ala = new HashMap<>();
                for (int i = 0; i < entry.formals.size(); ++i) {
                    String formal = entry.formals.get(i);
                    String actual = (i < actuals.size()) ? actuals.get(i) : "";
                    ala.put(formal, actual);
                }

                // Expand from MDT starting index until MEND
                int idx = entry.mdtStart;
                while (true) {
                    String mdtLine = mdt.get(idx);
                    if (mdtLine == null) {
                        throw new IOException("MDT missing index " + idx + " while expanding " + entry.name);
                    }
                    String trimmed = mdtLine.trim();
                    // Stop on MEND
                    if (trimmed.equalsIgnoreCase("MEND")) {
                        break;
                    }
                    // Replace formal occurrences with actuals
                    String expanded = substituteFormals(trimmed, ala);
                    bw.write(expanded);
                    bw.newLine();
                    idx++;
                }
            } else {
                // Normal line: write as-is
                bw.write(line);
                bw.newLine();
            }
        }

        bw.flush();
        bw.close();
    }

    // Substitute formal parameters in a line using the ALA map.
    private static String substituteFormals(String text, Map<String, String> ala) {
        String out = text;
        List<String> keys = new ArrayList<>(ala.keySet());
        keys.sort((a,b) -> Integer.compare(b.length(), a.length()));
        for (String formal : keys) {
            String actual = ala.get(formal);
            if (actual == null) actual = "";
            out = out.replace(formal, actual);
        }
        return out;
    }
}