# bully_compressed.py
# This is a compressed simulation of the Bully Algorithm.

class Process:
    def __init__(self, pid):
        self.id = pid
        self.is_active = True
        self.is_coord = False

class BullyAlgorithm:
    def __init__(self, n_proc):
        self.n_proc = n_proc
        self.procs = [Process(i + 1) for i in range(n_proc)]
        self.coord_id = n_proc
        self.procs[self.coord_id - 1].is_coord = True
        print(f"System initialized. P{self.coord_id} is coordinator.")
        self.print_state()

    def print_state(self):
        """Prints the current status of all processes."""
        print("--- System State ---")
        for p in self.procs:
            status = "Active" if p.is_active else "Inactive"
            coord = "(C)" if p.is_coord else ""
            print(f"  P{p.id}: {status} {coord}")
        print("--------------------")

    def bring_down(self, pid):
        """Simulates a process failing."""
        if not 1 <= pid <= self.n_proc or not self.procs[pid - 1].is_active:
            print(f"\nP{pid} is already down or invalid.")
            return
        self.procs[pid - 1].is_active = False
        print(f"\n>>> P{pid} has gone down. <<<")
        if self.procs[pid - 1].is_coord:
            self.procs[pid - 1].is_coord = False
            print("The coordinator has crashed!")

    def hold_election(self, init_id):
        """Simulates the election process initiated by a process."""
        if not self.procs[init_id - 1].is_active:
            print(f"P{init_id} is down, cannot start election.")
            return

        print(f"\n--- P{init_id} starts election ---")
        
        # Check if any higher-ID process is active
        higher_procs_active = False
        for i in range(init_id, self.n_proc):
            if self.procs[i].is_active:
                print(f"P{init_id} sends Election to P{i + 1}. (P{i+1} responds OK)")
                higher_procs_active = True
        
        if not higher_procs_active:
            # No higher-ID process is active, this initiator wins
            print(f"P{init_id} finds no higher active process.")
            self.set_new_coordinator(init_id)
            return

        # If higher processes were found, they take over
        # The first active higher-ID process will start its own election
        for i in range(init_id, self.n_proc):
            if self.procs[i].is_active:
                print(f"P{init_id} backs down.")
                self.hold_election(i + 1) # "Bully" aspect
                return # This initiator's election process is over

    def set_new_coordinator(self, new_cid):
        """Sets the new coordinator and announces it."""
        if self.coord_id:
            self.procs[self.coord_id - 1].is_coord = False
            
        self.coord_id = new_cid
        self.procs[new_cid - 1].is_coord = True
        print(f"\n>>> P{new_cid} is the new coordinator! <<<")
        
        # Announce victory to all other active processes
        for p in self.procs:
            if p.is_active and p.id != new_cid:
                print(f"P{new_cid} sends Victory to P{p.id}.")
        self.print_state()

if __name__ == "__main__":
    print("======== Bully Algorithm Simulation ========")
    try:
        n = int(input("Enter the total number of processes: "))
        if n < 2:
            print("At least 2 processes are needed."); exit()
    except ValueError:
        print("Invalid input."); exit()

    sim = BullyAlgorithm(n)
    
    # --- Simulation Scenario ---
    # 1. The coordinator (highest ID) crashes.
    sim.bring_down(n)
    sim.print_state()

    # 2. Process 1 detects the failure and starts an election.
    # (Using a fixed initiator instead of random)
    initiator = 1
    if not sim.procs[initiator-1].is_active:
        print(f"P{initiator} is down, cannot start election.")
    else:
        print(f"\nP{initiator} detects coordinator failure.")
        sim.hold_election(initiator)
    
    print("\n======== Simulation Complete ========")