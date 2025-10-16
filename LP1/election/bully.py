# bully.py
# This program simulates the Bully Algorithm for leader election in a distributed system.

import time
import random

class Process:
    def __init__(self, process_id):
        self.id = process_id
        self.is_active = True
        self.is_coordinator = False

class BullyAlgorithm:
    def __init__(self, num_processes):
        self.num_processes = num_processes
        # Initialize processes with unique IDs from 1 to num_processes
        self.processes = [Process(i + 1) for i in range(num_processes)]
        self.coordinator_id = num_processes  # Highest ID process starts as coordinator
        self.processes[self.coordinator_id - 1].is_coordinator = True
        print(f"System initialized with {num_processes} processes.")
        print(f"Process {self.coordinator_id} is the initial coordinator.")
        self.print_system_state()

    def print_system_state(self):
        """Prints the current status of all processes."""
        print("--- System State ---")
        for p in self.processes:
            status = "Active" if p.is_active else "Inactive"
            coord = "(Coordinator)" if p.is_coordinator else ""
            print(f"  Process {p.id}: {status} {coord}")
        print("--------------------")

    def bring_process_down(self, process_id):
        """Simulates a process failing or going offline."""
        if 1 <= process_id <= self.num_processes:
            if not self.processes[process_id - 1].is_active:
                print(f"\nProcess {process_id} is already down.")
                return
            self.processes[process_id - 1].is_active = False
            print(f"\n>>> Process {process_id} has gone down. <<<")
            if self.processes[process_id - 1].is_coordinator:
                self.processes[process_id - 1].is_coordinator = False
                print("The coordinator has crashed!")
        else:
            print(f"Invalid process ID: {process_id}")

    def hold_election(self, initiator_id):
        """Simulates the election process initiated by a specific process."""
        if not self.processes[initiator_id - 1].is_active:
            print(f"Process {initiator_id} is down and cannot start an election.")
            return

        print(f"\n--- Election Initiated by Process {initiator_id} ---")
        time.sleep(1)

        higher_processes_found = False
        # The initiator sends an 'Election' message to all processes with a higher ID.
        for i in range(initiator_id, self.num_processes):
            if self.processes[i].is_active:
                print(f"Process {initiator_id} sends Election message to Process {i + 1}.")
                time.sleep(0.5)
                # If a higher-ID process is active, it responds with 'OK'.
                print(f"Process {i + 1} responds with OK to Process {initiator_id}.")
                higher_processes_found = True

        if not higher_processes_found:
            # If no higher-ID process responds, the initiator becomes the new coordinator.
            print(f"Process {initiator_id} received no OK from higher processes.")
            print(f"Process {initiator_id} declares itself the new coordinator!")
            self.set_new_coordinator(initiator_id)
            return

        print(f"Process {initiator_id} will not become the coordinator and waits for a Victory message.")
        time.sleep(1)

        # Now, the higher processes that received an Election message will start their own elections.
        # This demonstrates the "bullying" aspect.
        for i in range(initiator_id, self.num_processes):
            if self.processes[i].is_active:
                self.hold_election(i + 1)
                # Once a higher process starts an election, we can stop this loop.
                return


    def set_new_coordinator(self, new_coordinator_id):
        """Sets the new coordinator and sends a 'Victory' message to all."""
        # Reset previous coordinator
        if self.coordinator_id and self.processes[self.coordinator_id - 1]:
             self.processes[self.coordinator_id - 1].is_coordinator = False
             
        self.coordinator_id = new_coordinator_id
        self.processes[new_coordinator_id - 1].is_coordinator = True
        print(f"\n>>> Announcing New Coordinator: Process {new_coordinator_id} <<<")
        
        # The new coordinator sends a 'Victory' (or 'Coordinator') message to all other active processes.
        for p in self.processes:
            if p.is_active and p.id != new_coordinator_id:
                print(f"Process {new_coordinator_id} sends Victory message to Process {p.id}.")
                time.sleep(0.3)
        self.print_system_state()


def simulate_bully():
    print("======== Bully Algorithm Simulation ========")
    try:
        num_p = int(input("Enter the total number of processes: "))
        if num_p < 2:
            print("At least 2 processes are needed.")
            return
    except ValueError:
        print("Invalid input. Please enter an integer.")
        return

    simulation = BullyAlgorithm(num_p)
    
    # --- Simulation Scenario ---
    time.sleep(2)
    
    # 1. The coordinator (highest ID) crashes.
    simulation.bring_process_down(num_p)
    simulation.print_system_state()
    time.sleep(2)

    # 2. A random active process detects the failure and starts an election.
    while True:
        random_process_id = random.randint(1, num_p -1)
        if simulation.processes[random_process_id-1].is_active:
            break

    print(f"\nProcess {random_process_id} detects coordinator failure and starts an election.")
    simulation.hold_election(random_process_id)
    
    print("\n======== Simulation Complete ========")

if __name__ == "__main__":
    simulate_bully()
