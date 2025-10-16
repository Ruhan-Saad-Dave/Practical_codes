import java.util.*;

// Base class for a Process
class Process {
    String pid;
    int arrivalTime, burstTime, remainingTime, priority;
    int waitingTime, turnaroundTime, completionTime;

    Process(String pid, int arrivalTime, int burstTime, int priority) {
        this.pid = pid;
        this.arrivalTime = arrivalTime;
        this.burstTime = burstTime;
        this.remainingTime = burstTime;
        this.priority = priority;
    }
}

// Abstract base class for all Scheduling Algorithms
abstract class Scheduler {
    List<Process> processes = new ArrayList<>();

    Scheduler(List<Process> processes) {
        this.processes = processes;
    }

    abstract void execute();
    abstract String getName();

    // Display result in tabular format
    void displayResults() {
        System.out.println("\n--- " + getName() + " Scheduling ---");
        System.out.println("PID\tAT\tBT\tPR\tCT\tTAT\tWT");
        for (Process p : processes) {
            System.out.println(p.pid + "\t" + p.arrivalTime + "\t" + p.burstTime + "\t" + p.priority +
                    "\t" + p.completionTime + "\t" + p.turnaroundTime + "\t" + p.waitingTime);
        }

        double avgTAT = processes.stream().mapToDouble(p -> p.turnaroundTime).average().orElse(0);
        double avgWT = processes.stream().mapToDouble(p -> p.waitingTime).average().orElse(0);
        System.out.printf("Average Turnaround Time: %.2f\n", avgTAT);
        System.out.printf("Average Waiting Time: %.2f\n", avgWT);
    }
}

// FCFS Scheduling
class FCFS extends Scheduler {
    FCFS(List<Process> processes) {
        super(processes);
    }

    @Override
    void execute() {
        processes.sort(Comparator.comparingInt(p -> p.arrivalTime));

        int currentTime = 0;
        for (Process p : processes) {
            currentTime = Math.max(currentTime, p.arrivalTime);
            currentTime += p.burstTime;
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
        }
    }

    @Override
    String getName() {
        return "FCFS";
    }
}

// SJF (Preemptive) Scheduling
class SJFPreemptive extends Scheduler {
    SJFPreemptive(List<Process> processes) {
        super(processes);
    }

    @Override
    void execute() {
        int time = 0, completed = 0;

        while (completed < processes.size()) {
            Process shortest = null;
            for (Process p : processes) {
                if (p.arrivalTime <= time && p.remainingTime > 0) {
                    if (shortest == null || p.remainingTime < shortest.remainingTime)
                        shortest = p;
                }
            }

            if (shortest == null) {
                time++;
                continue;
            }

            shortest.remainingTime--;
            time++;

            if (shortest.remainingTime == 0) {
                completed++;
                shortest.completionTime = time;
                shortest.turnaroundTime = shortest.completionTime - shortest.arrivalTime;
                shortest.waitingTime = shortest.turnaroundTime - shortest.burstTime;
            }
        }
    }

    @Override
    String getName() {
        return "SJF (Preemptive)";
    }
}

// Priority (Non-Preemptive) Scheduling
class PriorityNonPreemptive extends Scheduler {
    PriorityNonPreemptive(List<Process> processes) {
        super(processes);
    }

    @Override
    void execute() {
        List<Process> readyQueue = new ArrayList<>();
        List<Process> remaining = new ArrayList<>(processes);
        int time = 0;
        int completed = 0;

        while(completed < processes.size()){
            // Add arrived processes to the ready queue
            Iterator<Process> it = remaining.iterator();
            while(it.hasNext()){
                Process p = it.next();
                if(p.arrivalTime <= time){
                    readyQueue.add(p);
                    it.remove();
                }
            }

            if(readyQueue.isEmpty()){
                time++;
                continue;
            }

            // Sort by priority (lower number is higher priority)
            readyQueue.sort(Comparator.comparingInt(p -> p.priority));
            Process current = readyQueue.remove(0);

            time += current.burstTime;
            current.completionTime = time;
            current.turnaroundTime = current.completionTime - current.arrivalTime;
            current.waitingTime = current.turnaroundTime - current.burstTime;
            completed++;
        }
    }

    @Override
    String getName() {
        return "Priority (Non-Preemptive)";
    }
}

// Main class
public class CPUSchedule {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of processes: ");
        int n = sc.nextInt();

        List<Process> processList = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            System.out.println("Enter details for Process " + (i + 1) + " (AT BT Priority): ");
            int at = sc.nextInt();
            int bt = sc.nextInt();
            int pr = sc.nextInt();
            processList.add(new Process("P" + (i + 1), at, bt, pr));
        }
        sc.close();

        // Create copies for each algorithm
        List<Process> fcfsList = deepCopy(processList);
        List<Process> sjfList = deepCopy(processList);
        List<Process> priorityList = deepCopy(processList);

        Scheduler fcfs = new FCFS(fcfsList);
        fcfs.execute();
        fcfs.displayResults();

        Scheduler sjf = new SJFPreemptive(sjfList);
        sjf.execute();
        sjf.displayResults();

        Scheduler priority = new PriorityNonPreemptive(priorityList);
        priority.execute();
        priority.displayResults();
    }

    // Helper to deep copy process list
    static List<Process> deepCopy(List<Process> list) {
        List<Process> copy = new ArrayList<>();
        for (Process p : list)
            copy.add(new Process(p.pid, p.arrivalTime, p.burstTime, p.priority));
        return copy;
    }
}