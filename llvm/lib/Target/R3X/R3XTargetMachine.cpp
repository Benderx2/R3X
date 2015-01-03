#include "R3XTargetMachine.h"
#include "R3XTargetMachineObjectFile.h"
#include "R3X.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializeSparcTarget() {
  // Register the target.
  RegisterTargetMachine<R3XTargetMachine> X(R3XTarget);
}

/// SparcTargetMachine ctor - Create an ILP32 architecture model
///
R3XTargetMachine::R3XTargetMachine(const Target &T, StringRef TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       Reloc::Model RM, CodeModel::Model CM,
                                       CodeGenOpt::Level OL,
                                       bool is64bit)
  : LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL),
    TLOF(make_unique<SparcELFTargetObjectFile>()),
    Subtarget(TT, CPU, FS, *this, is64bit) {
  initAsmInfo();
}

R3XTargetMachine::~R3XTargetMachine() {}

namespace {
/// Sparc Code Generator Pass Configuration Options.
class R3XPassConfig : public TargetPassConfig {
public:
  SparcPassConfig(R3XTargetMachine *TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  R3XTargetMachine &getR3XTargetMachine() const {
    return getTM<R3XTargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *R3XTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new R3XPassConfig(this, PM);
}

void R3XPassConfig::addIRPasses() {
  addPass(createAtomicExpandPass(&getR3XTargetMachine()));

  TargetPassConfig::addIRPasses();
}

bool R3XPassConfig::addInstSelector() {
  addPass(createR3XISelDag(getR3XTargetMachine()));
  return false;
}

void R3XPassConfig::addPreEmitPass(){
  addPass(createR3XDelaySlotFillerPass(getR3XTargetMachine()));
}

void R3XTargetMachine::anchor() { }

R3XTargetMachine::R3XTargetMachine(const Target &T,
                                           StringRef TT, StringRef CPU,
                                           StringRef FS,
                                           const TargetOptions &Options,
                                           Reloc::Model RM,
                                           CodeModel::Model CM,
                                           CodeGenOpt::Level OL)
  : R3XTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, false) {
}

void R3XTargetMachine::anchor() { }

R3XTargetMachine::R3XTargetMachine(const Target &T,
                                           StringRef TT,  StringRef CPU,
                                           StringRef FS,
                                           const TargetOptions &Options,
                                           Reloc::Model RM,
                                           CodeModel::Model CM,
                                           CodeGenOpt::Level OL)
  : R3XTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, true) {
}