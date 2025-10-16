import java.util.*;

class MemoryBlock {
    int id;
    int size;
    boolean allocated;

    MemoryBlock(int id, int size) {
        this.id = id;
        this.size = size;
        this.allocated = false;
    }
}

class MyProcess {
    int id;
    int size;

    MyProcess(int id, int size) {
        this.id = id;
        this.size = size;
    }
}

public class eighth {

    static int nextFitPointer = 0;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of memory blocks: ");
        int m = sc.nextInt();
        List<MemoryBlock> blocks = new ArrayList<>();

        for (int i = 0; i < m; i++) {
            System.out.print("Enter size of block " + (i + 1) + ": ");
            blocks.add(new MemoryBlock(i + 1, sc.nextInt()));
        }

        System.out.print("\nEnter number of Processes: ");
        int n = sc.nextInt();
        List<MyProcess> processes = new ArrayList<>();

        for (int i = 0; i < n; i++) {
            System.out.print("Enter size of Process " + (i + 1) + ": ");
            processes.add(new MyProcess(i + 1, sc.nextInt()));
        }

        System.out.println("\n--- MEMORY ALLOCATION STRATEGIES ---");
        System.out.println("1. First Fit\n2. Next Fit\n3. Best Fit\n4. Worst Fit");
        System.out.print("Choose an option: ");
        int choice = sc.nextInt();

        // --- THIS IS THE FIXED PART ---
        switch (choice) {
            case 1:
                firstFit(new ArrayList<>(blocks), processes);
                break;
            case 2:
                nextFit(new ArrayList<>(blocks), processes);
                break;
            case 3:
                bestFit(new ArrayList<>(blocks), processes);
                break;
            case 4:
                worstFit(new ArrayList<>(blocks), processes);
                break;
            default:
                System.out.println("Invalid choice!");
                break;
        }

        sc.close();
    }

    // ---------- FIRST FIT ----------
    static void firstFit(List<MemoryBlock> blocks, List<MyProcess> processes) {
        System.out.println("\n--- FIRST FIT ---");
        for (MyProcess p : processes) {
            boolean allocated = false;
            for (MemoryBlock b : blocks) {
                if (!b.allocated && b.size >= p.size) {
                    b.allocated = true;
                    System.out.println("Process " + p.id + " allocated to Block " + b.id);
                    allocated = true;
                    break;
                }
            }
            if (!allocated)
                System.out.println("Process " + p.id + " not allocated");
        }
    }

    // ---------- NEXT FIT ----------
    static void nextFit(List<MemoryBlock> blocks, List<MyProcess> processes) {
        System.out.println("\n--- NEXT FIT ---");
        nextFitPointer = 0;

        for (MyProcess p : processes) {
            boolean allocated = false;
            int count = 0;

            while (count < blocks.size()) {
                MemoryBlock b = blocks.get(nextFitPointer);
                if (!b.allocated && b.size >= p.size) {
                    b.allocated = true;
                    System.out.println("Process " + p.id + " allocated to Block " + b.id);
                    allocated = true;
                    break;
                }
                nextFitPointer = (nextFitPointer + 1) % blocks.size();
                count++;
            }

            if (!allocated)
                System.out.println("Process " + p.id + " not allocated");
        }
    }

    // ---------- BEST FIT ----------
    static void bestFit(List<MemoryBlock> blocks, List<MyProcess> processes) {
        System.out.println("\n--- BEST FIT ---");
        for (MyProcess p : processes) {
            int bestIdx = -1;
            for (int i = 0; i < blocks.size(); i++) {
                MemoryBlock b = blocks.get(i);
                if (!b.allocated && b.size >= p.size) {
                    if (bestIdx == -1 || b.size < blocks.get(bestIdx).size)
                        bestIdx = i;
                }
            }

            if (bestIdx != -1) {
                blocks.get(bestIdx).allocated = true;
                System.out.println("Process " + p.id + " allocated to Block " + blocks.get(bestIdx).id);
            } else {
                System.out.println("Process " + p.id + " not allocated");
            }
        }
    }

    // ---------- WORST FIT ----------
    static void worstFit(List<MemoryBlock> blocks, List<MyProcess> processes) {
        System.out.println("\n--- WORST FIT ---");
        for (MyProcess p : processes) {
            int worstIdx = -1;
            for (int i = 0; i < blocks.size(); i++) {
                MemoryBlock b = blocks.get(i);
                if (!b.allocated && b.size >= p.size) {
                    if (worstIdx == -1 || b.size > blocks.get(worstIdx).size)
                        worstIdx = i;
                }
            }

            if (worstIdx != -1) {
                blocks.get(worstIdx).allocated = true;
                System.out.println("Process " + p.id + " allocated to Block " + blocks.get(worstIdx).id);
            } else {
                System.out.println("Process " + p.id + " not allocated");
            }
        }
    }
}