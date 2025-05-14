import os
import psutil
import subprocess
import time

spec_dir = "/work/johan/spec2017/benchspec/CPU/"
benches = [
    #"600.perlbench_s",
    #"602.gcc_s", 
    #"605.mcf_s",
    #"619.lbm_s",
    #"620.omnetpp_s",
    #"623.xalancbmk_s",
    #"625.x264_s",
    #"631.deepsjeng_s",
    "641.leela_s",
    #"644.nab_s",
    #"657.xz_s"
    ] #"638.imagick_s"]

processes = []

#for bench in benches:
#    env = os.environ.copy()

#    os.chdir("/work/johan/spec2017/")

#    while psutil.virtual_memory().percent > 60 and psutil.cpu_percent() > 85: time.sleep(30)
#    p = psutil.Popen(["./bin/runcpu", "--action", "runsetup", "--rebuild", "--config", "svfconfig", "--tune", "peak", bench])
#    # print("./bin/runcpu"+ " --action"+ " runsetup"+ " --rebuild"+ " --config"+ " svfconfig"+ " --tune"+ " peak"+ " "+ bench)
#    processes.append(p)

for p in processes:
    p.wait()
    
print("Completed compiling benchmarks to LLVM IR.....")

processes = []



def is_llvm_bitcode_file(filepath):
    """
    Check if the given file is an LLVM bitcode file by calling `file`.
    Returns True if the file appears to be LLVM bitcode, False otherwise.
    """
    try:
        result = subprocess.run(['file', filepath], capture_output=True, text=True)
        output = result.stdout.lower()
        return 'llvm ir bitcode' in output
    except Exception as e:
        print(f"Error checking file type: {e}")
        return False


print("All files verified as LLVM bitcode")

for bench in benches:
    if bench=="602.gcc_s":
        llvm_ir = "/work/johan/spec2017/benchspec/CPU/" + bench + "/build/build_peak_forSVF-64.0000/" + "sgcc"    
    else:
        llvm_ir = "/work/johan/spec2017/benchspec/CPU/" + bench + "/build/build_peak_forSVF-64.0000/" + bench.split(".")[-1]
    assert is_llvm_bitcode_file(llvm_ir)


for bench in benches:
    env = os.environ.copy()

    if bench=="602.gcc_s":
        llvm_ir = "/work/johan/spec2017/benchspec/CPU/" + bench + "/build/build_peak_forSVF-64.0000/" + "sgcc"    
    else:
        llvm_ir = "/work/johan/spec2017/benchspec/CPU/" + bench + "/build/build_peak_forSVF-64.0000/" + bench.split(".")[-1]
    instruction_list = "/work/johan/spec2017/benchspec/CPU/" + bench + "/run/InstructionsToInstrument2.txt"
    result_file = "/work/johan/spec2017/benchspec/CPU/" + bench + "/run/SVFResults4.txt"

    while psutil.virtual_memory().percent > 60 and psutil.cpu_percent() > 90: time.sleep(30)
    p = psutil.Popen(["/work/johan/SVF/example/src/svf-example", llvm_ir, instruction_list, result_file, bench])
    #print("/work/johan/SVF/example/src/svf-example", llvm_ir, instruction_list, result_file, bench)
    processes.append(p)

for p in processes:
    p.wait()

print("Completed analysis and generated SVF results")
