import java.io.*;
import java.util.*;

public class Pass2 {
    private static List<MNTEntry> mntList = new ArrayList<>();
    private static Map<String, MNTEntry> mntMap = new HashMap<>();
    private static List<String> mdt = new ArrayList<>();
    private static List<String> pntab = new ArrayList<>();
    private static List<KPDEntry> kpdtab = new ArrayList<>();

    static class MNTEntry {
        String n; int pp, kp, mdtp, kpdtp, pntabBase;
        MNTEntry(String n, int pp, int kp, int m, int k) { this.n = n; this.pp = pp; this.kp = kp; this.mdtp = m; this.kpdtp = k; }
    }
    static class KPDEntry { String n, v; KPDEntry(String n, String v) { this.n = n; this.v = v; } }

    public static void main(String[] args) throws IOException {
        loadTables();
        expandMacros();
        System.out.println("Macro Pass 2 complete. Check generated file: expanded.asm");
    }

    private static void loadTables() throws IOException {
        String line; int pntabCounter = 0;
        try (BufferedReader r = new BufferedReader(new FileReader("MNT.txt"))) {
            r.readLine(); r.readLine();
            while ((line = r.readLine()) != null) {
                String[] p = line.split("\t");
                MNTEntry e = new MNTEntry(p[1], Integer.parseInt(p[2]), Integer.parseInt(p[3]), Integer.parseInt(p[4]), Integer.parseInt(p[5]));
                e.pntabBase = pntabCounter; pntabCounter += e.pp + e.kp;
                mntList.add(e); mntMap.put(e.n, e);
            }
        }
        try (BufferedReader r = new BufferedReader(new FileReader("MDT.txt"))) {
            r.readLine(); r.readLine(); while ((line = r.readLine()) != null) mdt.add(line);
        }
        try (BufferedReader r = new BufferedReader(new FileReader("PNTAB.txt"))) {
            r.readLine(); r.readLine(); while ((line = r.readLine()) != null) pntab.add(line.split("\t")[1]);
        }
        try (BufferedReader r = new BufferedReader(new FileReader("KPDTAB.txt"))) {
            r.readLine(); r.readLine(); while ((line = r.readLine()) != null) { String[] p = line.split("\t"); kpdtab.add(new KPDEntry(p[1], p[2])); }
        }
    }

    private static void expandMacros() throws IOException {
        try (BufferedReader r = new BufferedReader(new FileReader("intermediate.asm"));
             FileWriter w = new FileWriter("expanded.asm")) {
            String line;
            while ((line = r.readLine()) != null) {
                String[] parts = line.split("\\s+", 2);
                if (mntMap.containsKey(parts[0])) processMacroCall(line, mntMap.get(parts[0]), w);
                else w.write(line + "\n");
            }
        }
    }

    private static void processMacroCall(String callLine, MNTEntry e, FileWriter w) throws IOException {
        String[] parts = callLine.split("\\s+", 2);
        String[] args = (parts.length > 1) ? parts[1].split(",") : new String[0];
        String[] apt = new String[pntab.size()];

        for (int i = 0; i < e.kp; i++) {
            apt[e.pntabBase + e.pp + i] = kpdtab.get(e.kpdtp + i).v;
        }

        for (int i = 0; i < args.length; i++) {
            String arg = args[i].trim();
            if (arg.contains("=")) {
                String[] kwp = arg.split("="); String name = kwp[0]; String val = kwp[1];
                for (int j = 0; j < e.kp; j++) {
                    int kpdIdx = e.kpdtp + j;
                    if (kpdtab.get(kpdIdx).n.equals(name)) { apt[e.pntabBase + e.pp + j] = val; break; }
                }
            } else {
                apt[e.pntabBase + i] = arg;
            }
        }

        for (int i = e.mdtp + 1; ; i++) {
            String mdtLine = mdt.get(i).split("\t", 2)[1];
            if (mdtLine.contains("MEND")) break;
            w.write("+" + substituteParams(mdtLine, apt) + "\n");
        }
    }

    private static String substituteParams(String mdtLine, String[] apt) {
        String[] parts = mdtLine.split("\\s+", 2);
        String opcode = parts[0];
        String operands = (parts.length > 1) ? parts[1] : "";
        String[] ops = operands.split(",");
        List<String> newOps = new ArrayList<>();
        
        for (String op : ops) {
            op = op.trim();
            if (op.startsWith("#")) newOps.add(apt[Integer.parseInt(op.substring(1))]);
            else newOps.add(op);
        }
        return opcode + " " + String.join(",", newOps);
    }
}