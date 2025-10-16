# ring.py
# This program simulates the Ring Algorithm for leader election in a distributed system.

import time
import random

class Process:
    def __init__(self, process_id):
        self.id = process_id
        self.is_active = True
        self.is_initiator = False

class RingAlgorithm:
    def __init__(self, num_processes):
        self.num_processes = num_processes
        # Initialize processes with unique IDs from 1 to num_processes
        self.processes = [Process(i + 1) for i in range(num_processes)]
        self.coordinator_id = num_processes # Highest ID process starts as coordinator
        print(f"System initialized with {num_processes} processes.")
        print(f"Process {self.coordinator_id} is the initial coordinator.")
        self.print_system_state()

    def print_system_state(self):
        """Prints the current status of all processes."""
        print("--- System State ---")
        active_processes = [p.id for p in self.processes if p.is_active]
        print(f"  Active Processes: {active_processes}")
        print(f"  Coordinator: {self.coordinator_id}")
        print("--------------------")

    def get_next_active_process(self, current_index):
        """Finds the next active process in the ring."""
        next_index = (current_index + 1) % self.num_processes
        while not self.processes[next_index].is_active:
            next_index = (next_index + 1) % self.num_processes
        return next_index

    def bring_process_down(self, process_id):
        """Simulates a process failing or going offline."""
        if 1 <= process_id <= self.num_processes:
            if not self.processes[process_id - 1].is_active:
                print(f"\nProcess {process_id} is already down.")
                return
            self.processes[process_id - 1].is_active = False
            print(f"\n>>> Process {process_id} has gone down. <<<")
        else:
            print(f"Invalid process ID: {process_id}")

    def hold_election(self, initiator_id):
        """Simulates the election process initiated by a specific process."""
        if not self.processes[initiator_id - 1].is_active:
            print(f"Process {initiator_id} is down and cannot start an election.")
            return

        print(f"\n--- Election Initiated by Process {initiator_id} ---")
        time.sleep(1)

        # The initiator creates an election message with its own ID.
        election_message = [initiator_id]
        initiator_index = initiator_id - 1
        current_index = initiator_index
        
        # The message is passed around the ring.
        while True:
            next_index = self.get_next_active_process(current_index)
            next_process = self.processes[next_index]
            
            print(f"Message {election_message} passed from Process {self.processes[current_index].id} to Process {next_process.id}.")
            time.sleep(0.7)
            
            # If the message gets back to the initiator, the circulation is complete.
            if next_process.id == initiator_id:
                print(f"Message has circulated back to the initiator, Process {initiator_id}.")
                break
            
            # Each process adds its own ID to the message.
            election_message.append(next_process.id)
            current_index = next_index
        
        # The initiator has the full list of active processes and elects the one with the highest ID.
        new_coordinator_id = max(election_message)
        print(f"\nElection complete. List of active processes: {election_message}")
        print(f"Process {initiator_id} elects Process {new_coordinator_id} as the new coordinator.")
        
        self.announce_new_coordinator(initiator_id, new_coordinator_id)

    def announce_new_coordinator(self, announcer_id, new_coordinator_id):
        """Announces the new coordinator to all processes in the ring."""
        self.coordinator_id = new_coordinator_id
        
        print(f"\n>>> Announcing New Coordinator: Process {new_coordinator_id} <<<")
        
        # A 'Coordinator' message is sent around the ring.
        announcement_message = f"Coordinator is {new_coordinator_id}"
        current_index = announcer_id - 1
        
        # Pass the announcement message around the ring to inform everyone.
        for _ in range(len([p for p in self.processes if p.is_active])):
            next_index = self.get_next_active_process(current_index)
            if self.processes[current_index].id == announcer_id and _ > 0:
                break # Stop once it has made a full circle
            print(f"Process {self.processes[current_index].id} sends announcement '{announcement_message}' to Process {self.processes[next_index].id}")
            time.sleep(0.5)
            current_index = next_index
            
        print("\nAnnouncement complete.")
        self.print_system_state()


def simulate_ring():
    print("======== Ring Algorithm Simulation ========")
    try:
        num_p = int(input("Enter the total number of processes: "))
        if num_p < 2:
            print("At least 2 processes are needed.")
            return
    except ValueError:
        print("Invalid input. Please enter an integer.")
        return

    simulation = RingAlgorithm(num_p)
    
    # --- Simulation Scenario ---
    time.sleep(2)
    
    # 1. The current coordinator crashes.
    print(f"\nSimulating crash of coordinator (Process {simulation.coordinator_id}).")
    simulation.bring_process_down(simulation.coordinator_id)
    
    # 2. Another random process might also crash to make it more interesting.
    if num_p > 2:
        random_crash_id = random.randint(1, num_p-1)
        simulation.bring_process_down(random_crash_id)

    simulation.print_system_state()
    time.sleep(2)

    # 3. A random active process detects the failure and starts an election.
    initiator_id = -1
    while initiator_id == -1:
        random_process_id = random.randint(1, num_p)
        if simulation.processes[random_process_id-1].is_active:
            initiator_id = random_process_id

    print(f"\nProcess {initiator_id} detects coordinator failure and starts an election.")
    simulation.hold_election(initiator_id)
    
    print("\n======== Simulation Complete ========")

if __name__ == "__main__":
    simulate_ring()
