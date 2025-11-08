import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

interface PRA { // PageReplacementAlgorithm
    // MODIFIED: Changed return type from int to void
    void run(int cap, int[] rs);
}

class LRU implements PRA {
    // MODIFIED: Changed return type, added print logic
    public void run(int cap, int[] rs) {
        int pf = 0; // pageFaults
        // fr (frames) list is used as a queue:
        // Index 0 = LRU, Index size-1 = MRU
        List<Integer> fr = new ArrayList<>(cap); 
        System.out.println("\n--- LRU Algorithm ---");
        System.out.print("Page\tFrames\n");
        
        for (int p : rs) {
            System.out.print(p + "\t");
            if (fr.contains(p)) {
                // --- HIT ---
                fr.remove(Integer.valueOf(p)); // Remove from middle
                fr.add(p); // Add to end (MRU)
                Page.printFrames(fr, cap);
                System.out.print("\tHit");
            } else {
                // --- FAULT ---
                pf++;
                if (fr.size() == cap) fr.remove(0); // Remove LRU (at front)
                fr.add(p); // Add new (at end)
                Page.printFrames(fr, cap);
                System.out.print("\tFault");
            }
            System.out.println(); // Newline for next step
        }
        System.out.println("\nTotal LRU Page Faults: " + pf);
    }
}

class Optimal implements PRA {
    // MODIFIED: Changed return type, added print logic
    public void run(int cap, int[] rs) {
        int pf = 0;
        List<Integer> fr = new ArrayList<>(cap); // fr (frames) is just a set
        System.out.println("\n--- Optimal Algorithm ---");
        System.out.print("Page\tFrames\n");
        
        for (int i = 0; i < rs.length; i++) {
            int p = rs[i];
            System.out.print(p + "\t");
            
            if (fr.contains(p)) {
                // --- HIT ---
                Page.printFrames(fr, cap);
                System.out.print("\tHit");
            } else {
                // --- FAULT ---
                pf++;
                if (fr.size() < cap) {
                    fr.add(p);
                } else {
                    int pte = findVictim(fr, rs, i); // pageToEvict
                    fr.remove(Integer.valueOf(pte));
                    fr.add(p);
                }
                Page.printFrames(fr, cap);
                System.out.print("\tFault");
            }
            System.out.println(); // Newline for next step
        }
        System.out.println("\nTotal Optimal Page Faults: " + pf);
    }

    private int findVictim(List<Integer> fr, int[] rs, int idx) {
        int pte = -1, fnu = -1; // pageToEvict, farthestNextUse
        for (int pif : fr) { // pageInFrame
            int nextUse = -1;
            for (int j = idx + 1; j < rs.length; j++) {
                if (rs[j] == pif) {
                    nextUse = j;
                    break;
                }
            }
            if (nextUse == -1) return pif; // Never used again
            if (nextUse > fnu) {
                fnu = nextUse;
                pte = pif;
            }
        }
        return (pte == -1) ? fr.get(0) : pte;
    }
}

public class Page {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of frames: ");
        int cap = sc.nextInt();
        sc.nextLine(); 
        System.out.print("Enter the page reference string (e.g., 7 0 1 2): ");
        int[] rs = Arrays.stream(sc.nextLine().split("\\s+"))
                         .mapToInt(Integer::parseInt)
                         .toArray();

        PRA lru = new LRU();
        PRA opt = new Optimal();
        
        // MODIFIED: Calls now just run the methods, no return value
        lru.run(cap, rs);
        opt.run(cap, rs);

        // MODIFIED: Removed final print block
        sc.close();
    }
    
    /**
     * NEW: Helper method to print the current state of the frames
     * in a clean, table-like format.
     */
    public static void printFrames(List<Integer> fr, int cap) {
        // Print all pages currently in frames
        for (int i = 0; i < fr.size(); i++) {
            System.out.print(fr.get(i) + "\t");
        }
        // Print placeholders for empty frames
        for (int i = 0; i < cap - fr.size(); i++) {
            System.out.print("-\t");
        }
    }
}