import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;

class Process {
    int pid, at, bt, prio, rbt, ft, wt, tat;
    public Process(int pid, int at, int bt, int pri) {
        this.pid = pid; this.at = at; this.bt = bt; this.prio = pri; this.rbt = bt;
    }
    public String toString() {
        return String.format("   %d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d", pid, at, bt, prio, ft, wt, tat);
    }
}

public class CPUSchedule {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        List<Process> pList = new ArrayList<>();
        System.out.print("Enter number of processes: ");
        int n = sc.nextInt();
        for (int i = 0; i < n; i++) {
            System.out.println("\n--- Process " + (i + 1) + " ---");
            System.out.print("Enter Arrival Time: "); int at = sc.nextInt();
            System.out.print("Enter Burst Time: "); int bt = sc.nextInt();
            System.out.print("Enter Priority: "); int pri = sc.nextInt();
            pList.add(new Process(i + 1, at, bt, pri));
        }
        runFCFS(new ArrayList<>(pList));
        runSJFPreemptive(new ArrayList<>(pList));
        runPriorityNonPreemptive(new ArrayList<>(pList));
        sc.close();
    }

    public static void runFCFS(List<Process> pList) {
        Collections.sort(pList, Comparator.comparingInt(p -> p.at));
        int ct = 0;
        for (Process p : pList) {
            if (ct < p.at) ct = p.at;
            p.ft = ct + p.bt;
            p.tat = p.ft - p.at;
            p.wt = p.tat - p.bt;
            ct = p.ft;
        }
        printResults(pList, "First-Come, First-Served (FCFS)");
    }

    public static void runSJFPreemptive(List<Process> pList) {
        int ct = 0, comp = 0, n = pList.size();
        while (comp < n) {
            Process sp = null; int minRem = Integer.MAX_VALUE;
            for (Process p : pList) {
                if (p.at <= ct && p.rbt > 0 && p.rbt < minRem) {
                    minRem = p.rbt; sp = p;
                }
            }
            if (sp == null) { ct++; } else {
                sp.rbt--; ct++;
                if (sp.rbt == 0) {
                    sp.ft = ct;
                    sp.tat = sp.ft - sp.at;
                    sp.wt = sp.tat - sp.bt;
                    comp++;
                }
            }
        }
        printResults(pList, "Shortest-Job-First (SJF) - Preemptive");
    }

    public static void runPriorityNonPreemptive(List<Process> pList) {
        int ct = 0, comp = 0, n = pList.size();
        while (comp < n) {
            Process hp = null; int maxPri = Integer.MAX_VALUE;
            for (Process p : pList) {
                if (p.at <= ct && p.rbt > 0 && p.prio < maxPri) {
                    maxPri = p.prio; hp = p;
                }
            }
            if (hp == null) { ct++; } else {
                ct += hp.bt;
                hp.ft = ct;
                hp.tat = hp.ft - hp.at;
                hp.wt = hp.tat - hp.bt;
                hp.rbt = 0;
                comp++;
            }
        }
        printResults(pList, "Priority (Non-Preemptive)");
    }

    public static void printResults(List<Process> pList, String name) {
        double totWT = 0, totTAT = 0; int n = pList.size();
        System.out.println("\n\n--- Results for: " + name + " ---");
        System.out.println("-----------------------------------------------------------------------------------------");
        System.out.println("  PID\t\tAT\t\tBT\t\tPriority\tFT\t\tWT\t\tTAT");
        System.out.println("-----------------------------------------------------------------------------------------");
        Collections.sort(pList, Comparator.comparingInt(p -> p.pid));
        for (Process p : pList) {
            System.out.println(p.toString());
            totWT += p.wt; totTAT += p.tat;
        }
        System.out.println("-----------------------------------------------------------------------------------------");
        System.out.printf("Average Waiting Time: %.2f\n", (totWT / n));
        System.out.printf("Average Turnaround Time: %.2f\n", (totTAT / n));
    }
}