import java.util.*;

// Abstract base class for page replacement algorithms
abstract class PageReplacement {
    protected int[] pages;
    protected int capacity;

    PageReplacement(int[] pages, int capacity) {
        this.pages = pages;
        this.capacity = capacity;
    }

    public abstract void simulate();
}

// ---------------- LRU Algorithm ----------------
class LRUPageReplacement extends PageReplacement {

    LRUPageReplacement(int[] pages, int capacity) {
        super(pages, capacity);
    }

    @Override
    public void simulate() {
        // Use LinkedHashSet to maintain insertion order for LRU logic
        LinkedHashSet<Integer> frames = new LinkedHashSet<>();
        int pageFaults = 0;

        System.out.println("\n--- LRU Page Replacement ---");
        for (int i = 0; i < pages.length; i++) {
            int page = pages[i];

            if (!frames.contains(page)) {
                if (frames.size() == capacity) {
                    // Remove the first element (the least recently used)
                    int lru = frames.iterator().next();
                    frames.remove(lru);
                }
                pageFaults++;
            } else {
                // If page is already in frames, remove it to re-insert at the end (most recent)
                frames.remove(page);
            }
            
            frames.add(page); // Add the current page to the end

            System.out.println("Step " + (i + 1) + " -> Page: " + page + " | Frames: " + frames);
        }

        System.out.println("Total Page Faults (LRU): " + pageFaults);
    }
}

// ---------------- Optimal Algorithm ----------------
class OptimalPageReplacement extends PageReplacement {

    OptimalPageReplacement(int[] pages, int capacity) {
        super(pages, capacity);
    }

    @Override
    public void simulate() {
        List<Integer> frames = new ArrayList<>();
        int pageFaults = 0;

        System.out.println("\n--- Optimal Page Replacement ---");
        for (int i = 0; i < pages.length; i++) {
            int page = pages[i];

            if (!frames.contains(page)) {
                if (frames.size() == capacity) {
                    int indexToReplace = predict(frames, pages, i + 1);
                    frames.set(indexToReplace, page);
                } else {
                    frames.add(page);
                }
                pageFaults++;
            }

            System.out.println("Step " + (i + 1) + " -> Page: " + page + " | Frames: " + frames);
        }

        System.out.println("Total Page Faults (Optimal): " + pageFaults);
    }

    private int predict(List<Integer> frames, int[] pages, int nextIndex) {
        int farthest = -1;
        int indexToReplace = -1;

        for (int i = 0; i < frames.size(); i++) {
            int framePage = frames.get(i);
            int j;
            for (j = nextIndex; j < pages.length; j++) {
                if (pages[j] == framePage) {
                    if (j > farthest) {
                        farthest = j;
                        indexToReplace = i;
                    }
                    break;
                }
            }
            // If a page is never used again, it's the perfect candidate to replace
            if (j == pages.length) {
                return i;
            }
        }
        
        // If all pages in frames are used again, replace the one used farthest in the future
        // If no page is found (shouldn't happen if frames are full), default to replacing the first frame
        return (indexToReplace == -1) ? 0 : indexToReplace;
    }
}

// ---------------- Main Driver Class ----------------
public class ninth {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of pages: ");
        int n = sc.nextInt();

        int[] pages = new int[n];
        System.out.println("Enter the page reference string (space-separated):");
        for (int i = 0; i < n; i++) {
            pages[i] = sc.nextInt();
        }

        System.out.print("Enter number of frames: ");
        int capacity = sc.nextInt();

        // Run LRU Simulation
        PageReplacement lru = new LRUPageReplacement(pages, capacity);
        lru.simulate();

        // Run Optimal Simulation
        PageReplacement optimal = new OptimalPageReplacement(pages, capacity);
        optimal.simulate();

        sc.close();
    }
}