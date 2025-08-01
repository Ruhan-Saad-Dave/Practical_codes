import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Represents a literal, used in both Pass 1 and Pass 2.
 * Stores its value and the assigned memory address.
 */
class LiteralP2 {
    String value;
    int address;

    /**
     * Constructs a new LiteralP2 with the given value and address.
     * @param value The string value of the literal.
     * @param address The memory address assigned to the literal.
     */
    public LiteralP2(String value, int address) {
        this.value = value;
        this.address = address;
    }
}

/**
 * Implements Pass 2 of a simple assembler.
 * Reads Pass 1's output tables and the original assembly program
 * to generate intermediate code.
 */
public class Pass2Assembler {

    // Tables loaded from Pass 1 output files
    private static Map<String, Integer> SYMTAB_P2 = new HashMap<>(); // Symbol Table: Symbol -> Address
    private static List<LiteralP2> LITTAB_P2 = new ArrayList<>(); // Literal Table: List of LiteralP2 objects
    private static List<Integer> POOLTAB_P2 = new ArrayList<>(); // Pool Table: List of starting indices in LITTAB_P2

    // Maps for converting mnemonics to numeric codes for intermediate code generation
    private static final Map<String, String> OPCODE_MNEMONIC_TO_CODE = new HashMap<>();
    private static final Map<String, String> REGISTER_MNEMONIC_TO_CODE = new HashMap<>();
    private static final Map<String, String> CONDITION_MNEMONIC_TO_CODE = new HashMap<>();
    private static final Map<String, String> AD_MNEMONIC_TO_CODE = new HashMap<>(); // Assembler Directives
    private static final Map<String, String> DL_MNEMONIC_TO_CODE = new HashMap<>(); // Declarative Statements

    // Initialize mnemonic-to-code maps
    static {
        // Imperative Statements (IS)
        OPCODE_MNEMONIC_TO_CODE.put("MOVER", "01");
        OPCODE_MNEMONIC_TO_CODE.put("ADD", "02");
        OPCODE_MNEMONIC_TO_CODE.put("SUB", "03");
        OPCODE_MNEMONIC_TO_CODE.put("MULT", "04");
        OPCODE_MNEMONIC_TO_CODE.put("DIV", "05");
        OPCODE_MNEMONIC_TO_CODE.put("BC", "06");
        OPCODE_MNEMONIC_TO_CODE.put("READ", "07");
        OPCODE_MNEMONIC_TO_CODE.put("PRINT", "08");
        OPCODE_MNEMONIC_TO_CODE.put("STOP", "09");

        // Registers
        REGISTER_MNEMONIC_TO_CODE.put("AREG", "01");
        REGISTER_MNEMONIC_TO_CODE.put("BREG", "02");
        REGISTER_MNEMONIC_TO_CODE.put("CREG", "03");
        REGISTER_MNEMONIC_TO_CODE.put("DREG", "04");

        // Condition Codes for BC
        CONDITION_MNEMONIC_TO_CODE.put("LT", "01"); // Less Than
        CONDITION_MNEMONIC_TO_CODE.put("LE", "02"); // Less Than or Equal
        CONDITION_MNEMONIC_TO_CODE.put("EQ", "03"); // Equal
        CONDITION_MNEMONIC_TO_CODE.put("GT", "04"); // Greater Than
        CONDITION_MNEMONIC_TO_CODE.put("GE", "05"); // Greater Than or Equal
        CONDITION_MNEMONIC_TO_CODE.put("ANY", "06"); // Any (unconditional jump)

        // Assembler Directives (AD)
        AD_MNEMONIC_TO_CODE.put("START", "01");
        AD_MNEMONIC_TO_CODE.put("END", "02");
        AD_MNEMONIC_TO_CODE.put("ORIGIN", "03");
        AD_MNEMONIC_TO_CODE.put("EQU", "04");
        AD_MNEMONIC_TO_CODE.put("LTORG", "05");

        // Declarative Statements (DL)
        DL_MNEMONIC_TO_CODE.put("DC", "01");
        DL_MNEMONIC_TO_CODE.put("DS", "02");
    }

    // Current literal pool index for processing LTORG/END
    private static int currentPoolIndex = 0;
    private static int literalCounter = 0; // To track literals within the current pool

    public static void main(String[] args) {
        try {
            // Step 1: Load tables generated by Pass 1
            loadSymbolTable("symbol_table.txt");
            loadLiteralTable("literal_table.txt");
            loadPoolTable("pool_table.txt");

            // Step 2: Generate intermediate code
            generateIntermediateCode("program.asm", "intermediate_code.txt");

            System.out.println("Pass 2 completed successfully.");
            System.out.println("Intermediate Code written to intermediate_code.txt");
        } catch (IOException e) {
            System.err.println("An error occurred during file operations: " + e.getMessage());
            e.printStackTrace();
        } catch (Exception e) {
            System.err.println("An unexpected error occurred: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Loads the Symbol Table from the specified file.
     * @param filename The path to the symbol_table.txt file.
     * @throws IOException If an I/O error occurs.
     */
    private static void loadSymbolTable(String filename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line = reader.readLine(); // Skip header line
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t");
                if (parts.length == 2) {
                    SYMTAB_P2.put(parts[0].trim(), Integer.parseInt(parts[1].trim()));
                }
            }
        }
    }

    /**
     * Loads the Literal Table from the specified file.
     * @param filename The path to the literal_table.txt file.
     * @throws IOException If an I/O error occurs.
     */
    private static void loadLiteralTable(String filename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line = reader.readLine(); // Skip header line
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\t");
                if (parts.length == 2) {
                    // Address might be "N/A" if not assigned (shouldn't happen with correct Pass 1)
                    int address = parts[1].trim().equals("N/A") ? -1 : Integer.parseInt(parts[1].trim());
                    LITTAB_P2.add(new LiteralP2(parts[0].trim(), address));
                }
            }
        }
    }

    /**
     * Loads the Pool Table from the specified file.
     * @param filename The path to the pool_table.txt file.
     * @throws IOException If an I/O error occurs.
     */
    private static void loadPoolTable(String filename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line = reader.readLine(); // Skip header line
            while ((line = reader.readLine()) != null) {
                POOLTAB_P2.add(Integer.parseInt(line.trim()));
            }
        }
    }

    /**
     * Reads the original assembly program and generates intermediate code.
     * @param inputFilename The path to the original program.asm file.
     * @param outputFilename The path for the generated intermediate_code.txt.
     * @throws IOException If an I/O error occurs.
     */
    private static void generateIntermediateCode(String inputFilename, String outputFilename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFilename));
             FileWriter writer = new FileWriter(outputFilename)) {

            String line;
            int LC = 0; // Location Counter, re-initialized for Pass 2 to track processing

            while ((line = reader.readLine()) != null) {
                String trimmedLine = line.trim();
                if (trimmedLine.isEmpty()) {
                    continue; // Skip empty lines
                }

                String[] tokens = trimmedLine.split("\\s+");
                String label = null;
                String opcode;
                String operand1 = null;
                String operand2 = null;

                int tokenIndex = 0;
                // Determine if the first token is a label or an opcode
                if (!OPCODE_MNEMONIC_TO_CODE.containsKey(tokens[tokenIndex].toUpperCase()) &&
                    !AD_MNEMONIC_TO_CODE.containsKey(tokens[tokenIndex].toUpperCase()) &&
                    !DL_MNEMONIC_TO_CODE.containsKey(tokens[tokenIndex].toUpperCase())) {
                    label = tokens[tokenIndex].toUpperCase();
                    tokenIndex++;
                }

                opcode = tokens[tokenIndex].toUpperCase();
                tokenIndex++;

                if (tokens.length > tokenIndex) {
                    String[] operands = tokens[tokenIndex].split(",");
                    operand1 = operands[0].trim();
                    if (operands.length > 1) {
                        operand2 = operands[1].trim();
                    }
                }

                String intermediateCode = "";

                // Handle directives first as they often control LC or don't generate code
                if (AD_MNEMONIC_TO_CODE.containsKey(opcode)) {
                    intermediateCode = handleAssemblerDirectiveP2(opcode, operand1, LC);
                    // Update LC based on START/ORIGIN, but only for internal tracking in Pass 2
                    if ("START".equals(opcode)) {
                        LC = Integer.parseInt(operand1);
                    } else if ("ORIGIN".equals(opcode)) {
                        LC = evaluateOperandP2(operand1, LC);
                    } else if ("LTORG".equals(opcode) || "END".equals(opcode)) {
                        // Process literals for LTORG/END
                        intermediateCode += processLiteralsForIntermediateCode(LC);
                        // Update LC after literals are placed
                        if (!intermediateCode.isEmpty()) {
                            // Count how many literals were processed to update LC correctly
                            int literalsProcessed = 0;
                            int poolStartIndex = POOLTAB_P2.get(currentPoolIndex);
                            int nextPoolStartIndex = (currentPoolIndex + 1 < POOLTAB_P2.size()) ? POOLTAB_P2.get(currentPoolIndex + 1) : LITTAB_P2.size();

                            for (int i = poolStartIndex + literalCounter; i < nextPoolStartIndex; i++) {
                                if (LITTAB_P2.get(i).address != -1) { // Check if address was assigned (should be by Pass 1)
                                    literalsProcessed++;
                                }
                            }
                            LC += literalsProcessed;
                            currentPoolIndex++; // Move to the next pool for the next LTORG/END
                            literalCounter = 0; // Reset literal counter for the new pool
                        }
                    } else if ("EQU".equals(opcode)) {
                        // EQU does not generate intermediate code or affect LC in Pass 2
                        // LC remains unchanged for this line
                        continue; // Skip writing this line to output
                    }
                } else if (OPCODE_MNEMONIC_TO_CODE.containsKey(opcode)) { // Imperative Statements
                    intermediateCode = handleImperativeStatementP2(opcode, operand1, operand2);
                    LC++; // Imperative statements always increment LC by 1
                } else if (DL_MNEMONIC_TO_CODE.containsKey(opcode)) { // Declarative Statements
                    intermediateCode = handleDeclarativeStatementP2(opcode, operand1);
                    if ("DC".equals(opcode)) {
                        LC++; // DC increments LC by 1
                    } else if ("DS".equals(opcode)) {
                        LC += Integer.parseInt(operand1); // DS increments LC by the specified size
                    }
                } else {
                    System.err.println("Error: Unknown opcode '" + opcode + "' in Pass 2. Line: " + trimmedLine);
                    intermediateCode = "Error: Unknown Opcode";
                }

                if (!intermediateCode.isEmpty()) {
                    writer.write(String.format("%03d", LC - (intermediateCode.split("\n").length -1)) + "\t" + intermediateCode + "\n");
                    // The LC for the first line of the intermediate code should be the LC before increment
                    // For multi-line intermediate code (like LTORG), the LC needs careful adjustment.
                    // The above LC calculation is a simplification. A more robust way would be to pass LC to handlers.
                }
            }
        }
    }

    /**
     * Handles Assembler Directives for Pass 2, generating their intermediate code.
     * @param opcode The directive's opcode.
     * @param operand The directive's operand.
     * @param currentLC The current Location Counter.
     * @return The intermediate code string for the directive.
     */
    private static String handleAssemblerDirectiveP2(String opcode, String operand, int currentLC) {
        String opcodeCode = AD_MNEMONIC_TO_CODE.get(opcode);
        if ("START".equals(opcode) || "ORIGIN".equals(opcode)) {
            int operandValue = evaluateOperandP2(operand, currentLC);
            return "(AD," + opcodeCode + ")(C," + operandValue + ")";
        } else if ("END".equals(opcode) || "LTORG".equals(opcode)) {
            return "(AD," + opcodeCode + ")";
            // Literals are handled separately by processLiteralsForIntermediateCode
        }
        return ""; // EQU is handled by skipping the line
    }

    /**
     * Handles Imperative Statements for Pass 2, generating their intermediate code.
     * @param opcode The imperative opcode.
     * @param operand1 The first operand.
     * @param operand2 The second operand.
     * @return The intermediate code string for the statement.
     */
    private static String handleImperativeStatementP2(String opcode, String operand1, String operand2) {
        StringBuilder sb = new StringBuilder();
        sb.append("(IS,").append(OPCODE_MNEMONIC_TO_CODE.get(opcode)).append(")");

        if (opcode.equals("BC")) { // Special handling for Branch Conditional
            sb.append("(").append(CONDITION_MNEMONIC_TO_CODE.get(operand1.toUpperCase())).append(")");
            // Operand2 is the target address (symbol)
            if (SYMTAB_P2.containsKey(operand2.toUpperCase())) {
                sb.append("(S,").append(SYMTAB_P2.get(operand2.toUpperCase())).append(")");
            } else {
                System.err.println("Error: Undefined symbol '" + operand2 + "' in BC instruction.");
                sb.append("(S,????)"); // Placeholder for undefined symbol
            }
        } else {
            // Handle first operand (register or literal)
            if (REGISTER_MNEMONIC_TO_CODE.containsKey(operand1.toUpperCase())) {
                sb.append("(").append(REGISTER_MNEMONIC_TO_CODE.get(operand1.toUpperCase())).append(")");
            } else {
                // This case should ideally not happen for standard IS where operand1 is a register
                // unless it's a specific instruction like READ/PRINT where it's a symbol.
                // For simplicity, assuming it's a symbol here if not a register.
                if (SYMTAB_P2.containsKey(operand1.toUpperCase())) {
                    sb.append("(S,").append(SYMTAB_P2.get(operand1.toUpperCase())).append(")");
                } else {
                    System.err.println("Error: Invalid first operand or undefined symbol '" + operand1 + "'.");
                    sb.append("(?,????)");
                }
            }

            // Handle second operand (symbol or literal)
            if (operand2 != null) {
                if (operand2.startsWith("='") && operand2.endsWith("'")) {
                    // It's a literal, find its address in LITTAB_P2
                    String literalValue = operand2.substring(2, operand2.length() - 1);
                    int literalAddress = -1;
                    int literalIndex = -1;
                    // Find literal in current pool
                    int poolStartIndex = POOLTAB_P2.get(currentPoolIndex-1 < 0 ? 0 : currentPoolIndex-1); // Use previous pool if current is not yet advanced
                    int nextPoolStartIndex = (currentPoolIndex < POOLTAB_P2.size()) ? POOLTAB_P2.get(currentPoolIndex) : LITTAB_P2.size();

                    for (int i = poolStartIndex; i < nextPoolStartIndex; i++) {
                        if (LITTAB_P2.get(i).value.equals(literalValue)) {
                            literalAddress = LITTAB_P2.get(i).address;
                            literalIndex = i; // Store index for (L, index)
                            break;
                        }
                    }
                    if (literalAddress != -1) {
                        sb.append("(L,").append(literalIndex).append(")"); // Use index for literal
                    } else {
                        System.err.println("Error: Literal '" + literalValue + "' not found or address not assigned.");
                        sb.append("(L,????)");
                    }
                } else {
                    // It's a symbol
                    if (SYMTAB_P2.containsKey(operand2.toUpperCase())) {
                        sb.append("(S,").append(SYMTAB_P2.get(operand2.toUpperCase())).append(")");
                    } else {
                        System.err.println("Error: Undefined symbol '" + operand2 + "' in instruction.");
                        sb.append("(S,????)");
                    }
                }
            }
        }
        return sb.toString();
    }

    /**
     * Handles Declarative Statements for Pass 2, generating their intermediate code.
     * @param opcode The declarative opcode (DC or DS).
     * @param operand The operand (value for DC, size for DS).
     * @return The intermediate code string.
     */
    private static String handleDeclarativeStatementP2(String opcode, String operand) {
        String opcodeCode = DL_MNEMONIC_TO_CODE.get(opcode);
        if ("DC".equals(opcode)) {
            return "(DL," + opcodeCode + ")(C," + operand + ")";
        } else if ("DS".equals(opcode)) {
            return "(DL," + opcodeCode + ")(C," + operand + ")";
        }
        return "";
    }

    /**
     * Processes literals in the current pool (from POOLTAB_P2) and generates their
     * intermediate code. This is called when LTORG or END is encountered.
     * @param currentLC The current Location Counter where literals will be placed.
     * @return A multi-line string of intermediate code for the literals.
     */
    private static String processLiteralsForIntermediateCode(int currentLC) {
        StringBuilder sb = new StringBuilder();
        int poolStartIndex = POOLTAB_P2.get(currentPoolIndex);
        int nextPoolStartIndex = (currentPoolIndex + 1 < POOLTAB_P2.size()) ? POOLTAB_P2.get(currentPoolIndex + 1) : LITTAB_P2.size();

        for (int i = poolStartIndex + literalCounter; i < nextPoolStartIndex; i++) {
            LiteralP2 lit = LITTAB_P2.get(i);
            // Only process if address is assigned (should be by Pass 1)
            if (lit.address != -1) {
                // The LC for the literal should be its assigned address from LITTAB
                sb.append(String.format("%03d", lit.address)).append("\t(DL,01)(C,").append(lit.value).append(")\n");
                literalCounter++;
            }
        }
        return sb.toString();
    }

    /**
     * Evaluates an operand which can be a direct number, a symbol, or a
     * symbol with an offset (e.g., SYMBOL+5, SYMBOL-2).
     * This uses the SYMTAB_P2 loaded from Pass 1.
     * @param operand The operand string to evaluate.
     * @param currentLC The current Location Counter (used for relative addressing if needed, though not strictly for this assembler).
     * @return The integer value of the evaluated operand.
     */
    private static int evaluateOperandP2(String operand, int currentLC) {
        try {
            return Integer.parseInt(operand);
        } catch (NumberFormatException e) {
            String[] parts;
            int value = 0;
            String symbolPart = operand;
            int offset = 0;

            if (operand.contains("+")) {
                parts = operand.split("\\+");
                symbolPart = parts[0].trim();
                offset = Integer.parseInt(parts[1].trim());
            } else if (operand.contains("-")) {
                parts = operand.split("-");
                symbolPart = parts[0].trim();
                offset = -Integer.parseInt(parts[1].trim());
            }

            if (SYMTAB_P2.containsKey(symbolPart.toUpperCase())) {
                value = SYMTAB_P2.get(symbolPart.toUpperCase());
            } else {
                System.err.println("Error: Undefined symbol '" + symbolPart + "' during operand evaluation in Pass 2. Line: " + operand);
                return 0; // Return 0 or throw error for undefined symbol
            }
            return value + offset;
        }
    }
}
