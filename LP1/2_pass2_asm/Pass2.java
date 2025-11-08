import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Pass2 {
    private static final List<Integer> symAddresses = new ArrayList<>();
    private static final List<LiteralData> litTable = new ArrayList<>();
    private static final List<Integer> pooltab = new ArrayList<>();

    static class LiteralData {
        String literal; 
        int address;   

        LiteralData(String literal, int address) {
            this.literal = literal;
            this.address = address;
        }
    }

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

    private static void loadTables() throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader("symtab.txt"))) {
            String line;
            reader.readLine();  //remove header
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t");
                symAddresses.add(Integer.parseInt(parts[1]));
            }
        }

        try (BufferedReader reader = new BufferedReader(new FileReader("littab.txt"))) {
            String line;
            reader.readLine(); 
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t");
                litTable.add(new LiteralData(parts[1], Integer.parseInt(parts[2])));
            }
        }

        try (BufferedReader reader = new BufferedReader(new FileReader("pooltab.txt"))) {
            String line;
            reader.readLine(); 
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t"); 
                pooltab.add(Integer.parseInt(parts[1]));
            }
        }
    }

    private static void generateTargetCode() throws IOException {
        try (BufferedReader icReader = new BufferedReader(new FileReader("intermediate_code.txt"));
             FileWriter targetWriter = new FileWriter("target_code.txt")) {

            int locationCounter = 0;
            int poolPtr = 0; 
            String icRegex = "^\\((\\w+), (\\d+)\\)\\s*(?:\\((\\d+)\\))?\\s*(?:\\((C|S|L), (\\d+)\\))?";
            Pattern pattern = Pattern.compile(icRegex);

            String line;
            while ((line = icReader.readLine()) != null) {
                Matcher m = pattern.matcher(line);
                if (!m.find()) continue;

                String type = m.group(1);  
                String code = m.group(2);  
                String reg = m.group(3);   
                String opType = m.group(4); 
                String opVal = m.group(5);  

                if ("AD".equals(type)) {
                    if ("00".equals(code)) { 
                        locationCounter = Integer.parseInt(opVal);
                    } else if ("01".equals(code) || "04".equals(code)) { 
                        locationCounter = processLiteralPool(targetWriter, locationCounter, poolPtr);
                        poolPtr++; 
                        if ("01".equals(code)) break; 
                    }
                } else if ("DL".equals(type)) {
                    int size = Integer.parseInt(opVal);
                    for(int i=0; i<size; i++) {
                        targetWriter.write(String.format("%d)\n", locationCounter));
                        locationCounter++;
                    }
                } else if ("IS".equals(type)) {
                    String opCodeStr = code;
                    String regStr = (reg != null) ? reg : "0"; 
                    int operandAddr = 0;

                    if (opType != null) {
                        int index = Integer.parseInt(opVal);
                        if ("S".equals(opType)) { 
                            operandAddr = symAddresses.get(index);
                        } else if ("L".equals(opType)) { 
                            operandAddr = litTable.get(index).address;
                        }
                    }
                    targetWriter.write(String.format("%d) + %s %s %03d\n",
                            locationCounter, opCodeStr, regStr, operandAddr));
                    locationCounter++;
                }
            }
        }
    }

    private static int processLiteralPool(FileWriter writer, int currentLc, int poolPtr) throws IOException {
        int lc = currentLc;     
        int poolStart = pooltab.get(poolPtr);       
        int poolEnd = (poolPtr + 1 < pooltab.size()) ? pooltab.get(poolPtr + 1) : litTable.size();

        for (int i = poolStart; i < poolEnd; i++) {
            LiteralData lit = litTable.get(i);
            String valueStr = lit.literal.replaceAll("='|'", "");
            int value = Integer.parseInt(valueStr);
            writer.write(String.format("%d) + 00 0 %03d\n", lc, value));
            lc++;
        }
        return lc;
    }
}