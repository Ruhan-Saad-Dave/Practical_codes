import java.util.Arrays;
import java.util.Scanner;

public class Fit {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of memory blocks: ");
        int nb = sc.nextInt();
        int[] blocks = new int[nb];
        for (int i = 0; i < nb; i++) {
            System.out.print("Enter size for block " + (i + 1) + ": ");
            blocks[i] = sc.nextInt();
        }
        System.out.print("\nEnter number of processes: ");
        int np = sc.nextInt();
        int[] proc = new int[np];
        for (int i = 0; i < np; i++) {
            System.out.print("Enter size for process " + (i + 1) + ": ");
            proc[i] = sc.nextInt();
        }
        firstFit(Arrays.copyOf(blocks, nb), proc);
        nextFit(Arrays.copyOf(blocks, nb), proc);
        bestFit(Arrays.copyOf(blocks, nb), proc);
        worstFit(Arrays.copyOf(blocks, nb), proc);
        sc.close();
    }

    public static void firstFit(int[] b, int[] p) {
        int[] alloc = new int[p.length]; 
        Arrays.fill(alloc, -1);
        for (int i = 0; i < p.length; i++) {
            for (int j = 0; j < b.length; j++) {
                if (b[j] >= p[i]) {
                    alloc[i] = j; 
                    b[j] -= p[i]; 
                    break;
                }
            }
        }
        printResults("First Fit", p, alloc);
    }

    public static void nextFit(int[] b, int[] p) {
        int[] alloc = new int[p.length]; 
        Arrays.fill(alloc, -1);
        int lastIdx = 0;
        for (int i = 0; i < p.length; i++) {
            for (int k = 0; k < b.length; k++) {
                int j = (lastIdx + k) % b.length;
                if (b[j] >= p[i]) {
                    alloc[i] = j; 
                    b[j] -= p[i]; 
                    lastIdx = (j + 1) % b.length; 
                    break;
                }
            }
        }
        printResults("Next Fit", p, alloc);
    }

    public static void bestFit(int[] b, int[] p) {
        int[] alloc = new int[p.length]; 
        Arrays.fill(alloc, -1);
        for (int i = 0; i < p.length; i++) {
            int bestIdx = -1; 
            int minFrag = Integer.MAX_VALUE;
            for (int j = 0; j < b.length; j++) {
                if (b[j] >= p[i]) {
                    int frag = b[j] - p[i];
                    if (frag < minFrag) { 
                        minFrag = frag; 
                        bestIdx = j; 
                    }
                }
            }
            if (bestIdx != -1) { 
                alloc[i] = bestIdx; 
                b[bestIdx] -= p[i]; 
            }
        }
        printResults("Best Fit", p, alloc);
    }

    public static void worstFit(int[] b, int[] p) {
        int[] alloc = new int[p.length]; 
        Arrays.fill(alloc, -1);
        for (int i = 0; i < p.length; i++) {
            int worstIdx = -1; 
            int maxFrag = -1;
            for (int j = 0; j < b.length; j++) {
                if (b[j] >= p[i]) {
                    int frag = b[j] - p[i];
                    if (frag > maxFrag) { 
                        maxFrag = frag; 
                        worstIdx = j; 
                    }
                }
            }
            if (worstIdx != -1) { 
                alloc[i] = worstIdx; 
                b[worstIdx] -= p[i]; 
            ~}
        }
        printResults("Worst Fit", p, alloc);
    }

    public static void printResults(String strat, int[] p, int[] alloc) {
        System.out.println("\n\n--- " + strat + " Allocation ---");
        System.out.println(" Process No.\tProcess Size\tBlock No.");
        for (int i = 0; i < p.length; i++) {
            System.out.print("     " + (i + 1) + "\t\t    " + p[i] + "\t\t");
            if (alloc[i] != -1) 
                System.out.println((alloc[i] + 1));
            else 
                System.out.println("Not Allocated");
        }
    }
}