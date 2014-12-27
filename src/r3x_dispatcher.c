#include <r3x_dispatcher.h>
#include <nt_malloc.h>
r3x_global_domain_t* r3x_init_domain(void) {
	r3x_global_domain_t* Domain = NULL; 
	Domain = nt_malloc(sizeof(r3x_global_domain_t));
	Domain->Jobs = nt_malloc(sizeof(r3x_job_t*)*16);
	Domain->CurrentJobID = 0;
	Domain->NumberOfActiveJobs = 0;
	Domain->TotalNumberOfJobs = 16;
	return Domain;
}
unsigned int r3x_dispatch_job(int InstructionPointer, int CycleUpdate, r3x_global_domain_t* Domain, bool ismain) {
	if(Domain->NumberOfActiveJobs >= Domain->TotalNumberOfJobs) { 
		Domain->Jobs = nt_realloc(Domain->Jobs, sizeof(r3x_job_t*) * (Domain->TotalNumberOfJobs + 16));
		Domain->TotalNumberOfJobs += 16;
	}
	for(unsigned int i = 0; i < Domain->TotalNumberOfJobs; i++) {
		if(Domain->Jobs[i] == NULL) {	
			Domain->Jobs[i] = nt_malloc(sizeof(r3x_job_t));
			Domain->Jobs[i]->Stack = Stack.Create();
			Domain->Jobs[i]->CallStack = Stack.Create();
			Stack.Push(Domain->Jobs[i]->Stack, 0);
			Domain->Jobs[i]->CycleUpdate = CycleUpdate;
			Domain->Jobs[i]->InstructionPointer = InstructionPointer;
			Domain->Jobs[i]->ismain = ismain;		
			Domain->Jobs[i]->JobID = i;
			Domain->Jobs[i]->paused = false;
			Domain->NumberOfActiveJobs++;
			return i;
		}	
	}
	printf("DISPATCHER: Unable to find free pointer. Please report this bug!\n");
	exit(0);
}
int r3x_load_job_state(r3x_cpu_t* CPU, r3x_global_domain_t* Domain) {
	if(Domain->NumberOfActiveJobs == 0) { 
		return -1;
	}
	if(Domain->CurrentJobID >= Domain->TotalNumberOfJobs) { 
		Domain->CurrentJobID = 0;
	}
	if(Domain->Jobs[Domain->CurrentJobID] != NULL) { 
		// Load Job State
		CPU->InstructionPointer = Domain->Jobs[Domain->CurrentJobID]->InstructionPointer;
		CPU->Stack = Domain->Jobs[Domain->CurrentJobID]->Stack;
		CPU->CallStack = Domain->Jobs[Domain->CurrentJobID]->Stack;
		for(int i = 0; i < MAX_NUMBER_OF_REGISTERS; i++) {
			CPU->Regs[i] = Domain->Jobs[Domain->CurrentJobID]->Regs[i];
		}
		return 0;
	}
	return -1;
}
void r3x_save_job_state(r3x_cpu_t* CPU, r3x_global_domain_t* Domain) { 
	if(Domain->Jobs[Domain->CurrentJobID] != NULL) {
		Domain->Jobs[Domain->CurrentJobID]->InstructionPointer = CPU->InstructionPointer;
		Domain->Jobs[Domain->CurrentJobID]->Stack = CPU->Stack;
		Domain->Jobs[Domain->CurrentJobID]->CallStack = CPU->CallStack;
		for(int i = 0; i < MAX_NUMBER_OF_REGISTERS; i++) {
			Domain->Jobs[Domain->CurrentJobID]->Regs[i] = CPU->Regs[i];		
		}
	}
}
void r3x_exit_job(r3x_global_domain_t* Domain) {
	Stack.DestroyStack(Domain->Jobs[Domain->CurrentJobID]->Stack);
	Stack.DestroyStack(Domain->Jobs[Domain->CurrentJobID]->CallStack); 
	nt_free(Domain->Jobs[Domain->CurrentJobID]);
	Domain->Jobs[Domain->CurrentJobID] = NULL;
	Domain->NumberOfActiveJobs--;
}
