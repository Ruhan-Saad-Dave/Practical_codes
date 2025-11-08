import java.io.*;
import java.util.*;

public class Pass1 {
    private static int lc = 0;
    private static Map<String, Integer> symtab = new LinkedHashMap<>();
    private static Map<String, Integer> littab = new LinkedHashMap<>();
    private static List<Integer> pooltab = new ArrayList<>(Arrays.asList(0));
    private static Map<String, OpcodeEntry> mot = new HashMap<>();

    static class OpcodeEntry { 
        String t, c; 
        int l; 
        
        OpcodeEntry(String t, String c, int l) { 
            this.t = t; 
            this.c = c; 
            this.l = l; 
        } 
    }

    static class AssemblyLine { 
        String l, o, o1, o2; 
        
        AssemblyLine(String l, String o, String o1, String o2) { 
            this.l = l; 
            this.o = o; 
            this.o1 = o1; 
            this.o2 = o2; 
        } 
    }

    static {
        mot.put("STOP", new OpcodeEntry("IS", "00", 1)); 
        mot.put("ADD", new OpcodeEntry("IS", "01", 1)); 
        mot.put("SUB", new OpcodeEntry("IS", "02", 1));
        mot.put("MOVER", new OpcodeEntry("IS", "04", 1)); 
        mot.put("MOVEM", new OpcodeEntry("IS", "05", 1)); 
        mot.put("DIV", new OpcodeEntry("IS", "08", 1));
        mot.put("READ", new OpcodeEntry("IS", "09", 1)); 
        mot.put("PRINT", new OpcodeEntry("IS", "10", 1)); 
        mot.put("START", new OpcodeEntry("AD", "00", 0));
        mot.put("END", new OpcodeEntry("AD", "01", 0)); 
        mot.put("LTORG", new OpcodeEntry("AD", "04", 0)); 
        mot.put("DS", new OpcodeEntry("DL", "01", 0));
    }

    public static void main(String[] args) throws IOException {
        assemble("input.txt"); 
        System.out.println("Pass 1 complete. Check generated files.");
    }

    private static int getSymbolIndex(String s) { 
        if (!symtab.containsKey(s)) 
            symtab.put(s, -1); 
        return new ArrayList<>(symtab.keySet()).indexOf(s); 
    }

    private static int getLiteralIndex(String l) { 
        if (!littab.containsKey(l)) 
            littab.put(l, -1); 
        return new ArrayList<>(littab.keySet()).indexOf(l); 
    }
    
    private static AssemblyLine parseLine(String line) {
        line = line.trim(); 
        if (line.isEmpty()) 
            return null;
            
        String[] tokens = line.split("\\s+");
        String label = null, opcode = null, op1 = null, op2 = null, opsStr = null;
        int opIdx = 0;
        if (mot.containsKey(tokens[0].toUpperCase())) { opcode = tokens[0]; opIdx = 0; }
        else {
            label = tokens[0];
            if (tokens.length > 1) { opcode = tokens[1]; opIdx = 1; }
            else return new AssemblyLine(label, null, null, null);
        }
        if (tokens.length > opIdx + 1) {
            StringBuilder sb = new StringBuilder();
            for (int i = opIdx + 1; i < tokens.length; i++) sb.append(tokens[i]);
            opsStr = sb.toString();
        }
        if (opsStr != null) {
            String[] operands = opsStr.split(",", 2); 
            op1 = operands[0].trim(); 
            if (operands.length > 1) op2 = operands[1].trim(); 
        }
        return new AssemblyLine(label, opcode, op1, op2);
    }

    private static void processLiteralPool(FileWriter iw) throws IOException {
        int poolStart = pooltab.get(pooltab.size() - 1);
        List<String> litNames = new ArrayList<>(littab.keySet());
        for (int i = poolStart; i < litNames.size(); i++) {
            littab.put(litNames.get(i), lc); lc++;
        }
        pooltab.add(littab.size());
    }

    public static void assemble(String inFile) throws IOException {
        try (BufferedReader r = new BufferedReader(new FileReader(inFile));
             FileWriter iw = new FileWriter("intermediate_code.txt");
             FileWriter sw = new FileWriter("symtab.txt");
             FileWriter lw = new FileWriter("littab.txt");
             FileWriter pw = new FileWriter("pooltab.txt")) {
            String line;
            while ((line = r.readLine()) != null) {
                AssemblyLine al = parseLine(line);
                if (al == null) continue;
                if (al.l != null) {
                    if (symtab.containsKey(al.l) && symtab.get(al.l) != -1) System.err.println("Dup sym: " + al.l);
                    symtab.put(al.l, lc);
                }
                if (al.o == null) continue;
                OpcodeEntry op = mot.get(al.o.toUpperCase());
                if (op == null) { System.err.println("Bad op: " + al.o); continue; }

                if (al.o.equalsIgnoreCase("START")) {
                    lc = Integer.parseInt(al.o1);
                    iw.write(String.format("(%s, %s) (C, %s)\n", op.t, op.c, al.o1));
                } else if (al.o.equalsIgnoreCase("LTORG")) {
                    iw.write(String.format("(%s, %s)\n", op.t, op.c));
                    processLiteralPool(iw);
                } else if (al.o.equalsIgnoreCase("END")) {
                    iw.write(String.format("(%s, %s)\n", op.t, op.c));
                    processLiteralPool(iw); break;
                } else if (al.o.equalsIgnoreCase("DS")) {
                    int size = Integer.parseInt(al.o1);
                    iw.write(String.format("(%s, %s) (C, %d)\n", op.t, op.c, size));
                    lc += size;
                } else if (op.t.equals("IS")) {
                    String rg = "", opd = "";
                    if (al.o.equals("MOVER") || al.o.equals("MOVEM") || al.o.equals("ADD") || al.o.equals("SUB") || al.o.equals("DIV")) {
                        rg = (al.o1 != null && al.o1.equalsIgnoreCase("BREG")) ? "(1)" : "(0)";
                        if (al.o2 != null) opd = al.o2.startsWith("=") ? String.format("(L, %02d)", getLiteralIndex(al.o2)) : String.format("(S, %02d)", getSymbolIndex(al.o2));
                    } else if (al.o.equals("READ") || al.o.equals("PRINT")) {
                        if (al.o1 != null) opd = String.format("(S, %02d)", getSymbolIndex(al.o1));
                    }
                    iw.write(String.format("(%s, %s) %s %s\n", op.t, op.c, rg, opd).trim().replaceAll(" +", " ") + "\n");
                    lc += op.l;
                }
            }
            sw.write("Symb\tAddr\n");
            for (Map.Entry<String, Integer> e : symtab.entrySet()) sw.write(String.format("%s\t%d\n", e.getKey(), e.getValue()));
            lw.write("Lit#\tLit\tAddr\n");
            List<String> litNames = new ArrayList<>(littab.keySet());
            for (int i = 0; i < litNames.size(); i++) lw.write(String.format("%02d\t%s\t%d\n", i, litNames.get(i), littab.get(litNames.get(i))));
            pw.write("Pool#\tPool Base\n");
            for (int i = 0; i < pooltab.size() - 1; i++) pw.write(String.format("%02d\t%d\n", i, pooltab.get(i)));
        }
    }
}