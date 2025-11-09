# ring_compressed.py
# A compressed simulation of the Ring Algorithm.

import random

class Process:
    def __init__(self, pid):
        self.id = pid
        self.active = True

class RingAlgorithm:
    def __init__(self, n_proc):
        self.n_proc = n_proc
        self.procs = [Process(i + 1) for i in range(n_proc)]
        self.coord_id = n_proc
        print(f"System initialized. P{self.coord_id} is coordinator.")
        self.print_state()

    def print_state(self):
        """Prints the current status of all processes."""
        print("--- System State ---")
        active_p = [p.id for p in self.procs if p.active]
        print(f"  Active: {active_p}\n  Coordinator: {self.coord_id}\n--------------------")

    def get_next_active(self, curr_idx):
        """Finds the next active process in the ring."""
        next_idx = (curr_idx + 1) % self.n_proc
        # Loop until an active process is found
        while not self.procs[next_idx].active:
            next_idx = (next_idx + 1) % self.n_proc
        return next_idx

    def bring_down(self, pid):
        """Simulates a process failing."""
        if 1 <= pid <= self.n_proc and self.procs[pid - 1].active:
            self.procs[pid - 1].active = False
            print(f"\n>>> P{pid} has gone down. <<<")
        else:
            print(f"\nP{pid} is already down or invalid.")

    def hold_election(self, init_id):
        """Simulates the election process initiated by a process."""
        if not self.procs[init_id - 1].active:
            print(f"P{init_id} is down, cannot start election."); return

        print(f"\n--- P{init_id} starts election ---")
        msg = [init_id]
        curr_idx = init_id - 1
        
        # Pass the message around the ring
        while True:
            next_idx = self.get_next_active(curr_idx)
            next_p = self.procs[next_idx]
            
            print(f"Msg {msg} from P{self.procs[curr_idx].id} to P{next_p.id}.")
            
            # If it gets back to the initiator, stop
            if next_p.id == init_id:
                break
            
            # Add the current process's ID to the message
            msg.append(next_p.id)
            curr_idx = next_idx
        
        # Election is over, find the highest ID in the list
        new_coord = max(msg)
        print(f"\nElection complete. Active list: {msg}")
        print(f"P{init_id} elects P{new_coord} as coordinator.")
        
        self.announce(init_id, new_coord)

    def announce(self, announcer_id, new_coord_id):
        """Announces the new coordinator to all processes."""
        self.coord_id = new_coord_id
        print(f"\n>>> Announcing New Coordinator: P{new_coord_id} <<<")
        
        ann_msg = f"Coordinator is {new_coord_id}"
        curr_idx = announcer_id - 1
        start_id = self.procs[curr_idx].id
        
        # Pass the announcement message around the full ring
        while True:
            next_idx = self.get_next_active(curr_idx)
            print(f"P{self.procs[curr_idx].id} sends '{ann_msg}' to P{self.procs[next_idx].id}")
            curr_idx = next_idx
            if self.procs[curr_idx].id == start_id:
                break # Stop once it has made a full circle
                
        print("\nAnnouncement complete.")
        self.print_state()

def simulate_ring():
    print("======== Ring Algorithm Simulation ========")
    try:
        n = int(input("Enter total processes: "))
        if n < 2: print("Need at least 2."); return
    except ValueError: print("Invalid input."); return

    sim = RingAlgorithm(n)
    
    # --- Simulation Scenario ---
    # 1. Crash the coordinator
    print(f"\nCrashing coordinator (P{sim.coord_id}).")
    sim.bring_down(sim.coord_id)
    
    # 2. Crash another random process
    if n > 2:
        r_id = random.randint(1, n-1)
        sim.bring_down(r_id)
    sim.print_state()

    # 3. Find a random *active* process to start the election
    init_id = -1
    while init_id == -1:
        r_id = random.randint(1, n)
        if sim.procs[r_id-1].active:
            init_id = r_id

    print(f"\nP{init_id} detects failure and starts election.")
    sim.hold_election(init_id)
    print("\n======== Simulation Complete ========")

if __name__ == "__main__":
    simulate_ring()