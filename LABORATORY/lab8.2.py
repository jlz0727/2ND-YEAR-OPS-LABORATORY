class MemoryManager:
    def __init__(self):
        self.total_memory = 0
        self.processes = []
        self.remaining_memory = 0

    def run(self):
        self.total_memory = int(input("Enter the total memory available (in Bytes)-- "))
        self.remaining_memory = self.total_memory

        i = 0
        while True:
            mem_req = int(input(f"\nEnter memory required for process {i + 1} (in Bytes) -- "))
            if mem_req <= self.remaining_memory:
                print(f"\nMemory is allocated for Process {i + 1}")
                self.remaining_memory -= mem_req
                self.processes.append(mem_req)
            else:
                print("\nMemory is Full")
                break

            ch = input("Do you want to continue(y/n) -- ").strip().lower()
            if ch != 'y':
                break
            i += 1

        self.display_results()

    def display_results(self):
        print(f"\n\nTotal Memory Available -- {self.total_memory}")
        print("\n\tPROCESS\t\t MEMORY ALLOCATED")
        for idx, mem in enumerate(self.processes):
            print(f"\n \t{idx + 1}\t\t{mem}")

        total_allocated = self.total_memory - self.remaining_memory
        print(f"\n\nTotal Memory Allocated is {total_allocated}")
        print(f"Total External Fragmentation is {self.remaining_memory}")


#FOR RUNNING THE PROGRAM
if __name__ == "__main__":
    manager = MemoryManager()
    manager.run()