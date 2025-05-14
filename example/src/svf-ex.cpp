// //===- svf-ex.cpp -- A driver example of SVF-------------------------------------//
// //
// //                     SVF: Static Value-Flow Analysis
// //
// // Copyright (C) <2013->  <Yulei Sui>
// //

// // This program is free software: you can redistribute it and/or modify
// // it under the terms of the GNU General Public License as published by
// // the Free Software Foundation, either version 3 of the License, or
// // (at your option) any later version.

// // This program is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// // GNU General Public License for more details.

// // You should have received a copy of the GNU General Public License
// // along with this program.  If not, see <http://www.gnu.org/licenses/>.
// //
// //===-----------------------------------------------------------------------===//

// /*
//  // A driver program of SVF including usages of SVF APIs
//  //
//  // Author: Yulei Sui,
//  */

#include "SVF-LLVM/LLVMUtil.h"
#include "Graphs/SVFG.h"
#include "WPA/Andersen.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/Options.h"
#include <iostream>

using namespace llvm;
using namespace std;
using namespace SVF;

// /*!
//  * An example to query alias results of two LLVM values
//  */
// SVF::AliasResult aliasQuery(PointerAnalysis* pta, const SVFVar* v1, const SVFVar* v2)
// {
//     return pta->alias(v1->getId(), v2->getId());
// }

// /*!
//  * An example to print points-to set of an LLVM value
//  */
// std::string printPts(PointerAnalysis* pta, const SVFVar* svfval)
// {

//     std::string str;
//     raw_string_ostream rawstr(str);

//     NodeID pNodeId = svfval->getId();
//     const PointsTo& pts = pta->getPts(pNodeId);
//     for (PointsTo::iterator ii = pts.begin(), ie = pts.end();
//             ii != ie; ii++)
//     {
//         rawstr << " " << *ii << " ";
//         PAGNode* targetObj = pta->getPAG()->getGNode(*ii);
//         rawstr << "(" << targetObj->toString() << ")\t ";
//     }

//     return rawstr.str();

// }


// /*!
//  * An example to query/collect all successor nodes from a ICFGNode (iNode) along control-flow graph (ICFG)
//  */
// void traverseOnICFG(ICFG* icfg, const Instruction* inst)
// {
//     const ICFGNode* iNode = LLVMModuleSet::getLLVMModuleSet()->getICFGNode(inst);

//     FIFOWorkList<const ICFGNode*> worklist;
//     Set<const ICFGNode*> visited;
//     worklist.push(iNode);

//     /// Traverse along VFG
//     while (!worklist.empty())
//     {
//         const ICFGNode* iNode = worklist.pop();
//         for (ICFGNode::const_iterator it = iNode->OutEdgeBegin(), eit =
//                     iNode->OutEdgeEnd(); it != eit; ++it)
//         {
//             ICFGEdge* edge = *it;
//             ICFGNode* succNode = edge->getDstNode();
//             if (visited.find(succNode) == visited.end())
//             {
//                 visited.insert(succNode);
//                 worklist.push(succNode);
//             }
//         }
//     }
// }

// void dummyVisit(const VFGNode* node)
// {

// }
// /*!
//  * An example to query/collect all the uses of a definition of a value along value-flow graph (VFG)
//  */
// void traverseOnVFG(const SVFG* vfg, const SVFVar* svfval)
// {
//     if (!vfg->hasDefSVFGNode(svfval))
//         return;
//     const VFGNode* vNode = vfg->getDefSVFGNode(svfval);
//     FIFOWorkList<const VFGNode*> worklist;
//     Set<const VFGNode*> visited;
//     worklist.push(vNode);

//     /// Traverse along VFG
//     while (!worklist.empty())
//     {
//         const VFGNode* vNode = worklist.pop();
//         for (VFGNode::const_iterator it = vNode->OutEdgeBegin(), eit =
//                     vNode->OutEdgeEnd(); it != eit; ++it)
//         {
//             VFGEdge* edge = *it;
//             VFGNode* succNode = edge->getDstNode();
//             if (visited.find(succNode) == visited.end())
//             {
//                 visited.insert(succNode);
//                 worklist.push(succNode);
//             }
//         }
//     }

//     /// Collect all LLVM Values
//     for(Set<const VFGNode*>::const_iterator it = visited.begin(), eit = visited.end(); it!=eit; ++it)
//     {
//         const VFGNode* node = *it;
//         dummyVisit(node);
//         /// can only query VFGNode involving top-level pointers (starting with % or @ in LLVM IR)
//         /// PAGNode* pNode = vfg->getLHSTopLevPtr(node);
//         /// Value* val = pNode->getValue();
//     }
// }


#include "SVF-LLVM/SVFIRBuilder.h"
#include "SVF-LLVM/LLVMUtil.h"
#include "WPA/Andersen.h"
#include "WPA/WPAFSSolver.h"
#include "WPA/VersionedFlowSensitive.h"
#include "WPA/TypeAnalysis.h"

using namespace SVF;

// void runAliasAnalysis(PointerAnalysis* pta, const std::vector<const LoadStmt*>& loadInsts, const std::vector<const StoreStmt*>& storeInsts, const std::string& analysisName) {
//     llvm::outs() << "\n=== Running Alias Analysis: " << analysisName << " ===\n";
//     int count = 0;
//     for (const LoadStmt* load : loadInsts) {
//         // Get the corresponding LLVM instruction
//         // if (const llvm::Instruction* inst = dyn_cast<llvm::Instruction>(load->getValue())) {
//         //     // Retrieve the debug location
//         //     llvm::DebugLoc debugLoc = inst->getDebugLoc();
            
//         //     // Print the debug location information
//         //     if (debugLoc) {
//         //         llvm::outs() << "File: " << debugLoc.get()->getScope()->getFilename() 
//         //                     << ", Line " << debugLoc.getLine() + ":" + debugLoc.getCol() << "\n";
//         //     } else {
//         //         llvm::outs() << "No debug location available\n";
//         //     }
//         // }
//         // llvm::outs() << "SourceLoc: " << load->getValue()->getSourceLoc() << "\n";
//         for (const StoreStmt* store : storeInsts) {
//             SVF::AliasResult result = pta->alias(load->getRHSVarID(), store->getLHSVarID());
//             std::string aliasStr = "Unknown";
//             switch (result) {
//                 case SVF::NoAlias: aliasStr = "NoAlias"; break;
//                 case SVF::MayAlias: aliasStr = "MayAlias"; break;
//                 case SVF::MustAlias: aliasStr = "MustAlias"; break;
//                 default: break;
//             }
//             // if (aliasStr != "NoAlias") {
//             if (aliasStr == "MustAlias") {
//                 count++;
//                 break;
//                 // llvm::outs() << "Alias result (" << analysisName << ") between:\n  Load @ "
//                 //              << load->toString() << "\n  and Store @ "
//                 //              << store->toString() << ": " << aliasStr << "\n";
//             }
//         }
//     }
//     llvm::outs() << "Total MayAlias Count = "<< count++ <<"\n";
// }


// int main(int argc, char ** argv) {
//     std::vector<std::string> moduleNameVec;
//     moduleNameVec = OptionBase::parseOptions(
//             argc, argv, "Whole Program Points-to Analysis", "[options] <input-bitcode...>"
//     );

//     if (Options::WriteAnder() == "ir_annotator") {
//         LLVMModuleSet::preProcessBCs(moduleNameVec);
//     }

//     LLVMModuleSet::buildSVFModule(moduleNameVec);

//     // Build SVF IR
//     SVFIRBuilder builder;
//     SVFIR* pag = builder.build();

//     // === STEP 1: Collect Load and Store instructions
//     std::vector<const LoadStmt*> loadInsts;
//     std::vector<const StoreStmt*> storeInsts;

//     ICFG* icfg = pag->getICFG();
//     for (auto it = icfg->begin(); it != icfg->end(); ++it) {
//         const ICFGNode* icfgNode = it->second;
//         if (!pag->hasSVFStmtList(icfgNode))
//             continue;

//         SVFIR::SVFStmtList& stmtList = pag->getSVFStmtList(icfgNode);
//         for (const SVFStmt* stmt : stmtList) {
//             if (const LoadStmt* load = SVFUtil::dyn_cast<LoadStmt>(stmt)) {
//                 loadInsts.push_back(load);
//             } else if (const StoreStmt* store = SVFUtil::dyn_cast<StoreStmt>(stmt)) {
//                 storeInsts.push_back(store);
//             }
//         }
//     }

//     // === STEP 2: Run each pointer analysis

//     //  // Andersen
//     //  {
//     //     AndersenSFR* ander = AndersenSFR::createAndersenSFR(pag);
//     //     runAliasAnalysis(ander, loadInsts, storeInsts, "AndersenSVF");
//     //     AndersenSFR::releaseAndersenSFR();
//     // }

//     //  // Andersen
//     //  {
//     //     AndersenSCD* ander = AndersenSCD::createAndersenSCD(pag);
//     //     runAliasAnalysis(ander, loadInsts, storeInsts, "AndersenSCD");
//     //     AndersenSCD::releaseAndersenSCD();
//     // }
    
//     // Andersen
//     {
//         Andersen* ander = AndersenWaveDiff::createAndersenWaveDiff(pag);
//         runAliasAnalysis(ander, loadInsts, storeInsts, "AndersenWaveDiff");
//         AndersenWaveDiff::releaseAndersenWaveDiff();
//     }

//     // // FSSPARSE_WPA (Field-sensitive flow-insensitive analysis)
//     // {
//     //     FSSPARSE* fspta = FSSPARSE::createFSSPARSE(pag);
//     //     runAliasAnalysis(fspta, loadInsts, storeInsts, "FSSPARSE_WPA");
//     //     FSSPARSE::releaseFSSPARSE();
//     // }

//     // VFS_WPA (Value-flow-sensitive analysis)
//     {
//         VersionedFlowSensitive* vfspta = VersionedFlowSensitive::createVFSWPA(pag);
//         runAliasAnalysis(vfspta, loadInsts, storeInsts, "VFS_WPA");
//         VersionedFlowSensitive::releaseVFSWPA();
//     }

//     // // TypeCPP_WPA (Type-based pointer analysis)
//     // {
//     //     TypeBasedPointerAnalysis* typepta = TypeBasedPointerAnalysis::createTypeBasedPointerAnalysis(pag);
//     //     runAliasAnalysis(typepta, loadInsts, storeInsts, "TypeCPP_WPA");
//     //     TypeBasedPointerAnalysis::releaseTypeBasedPointerAnalysis();
//     // }

//     // === Cleanup
//     SVFIR::releaseSVFIR();
//     LLVMModuleSet::getLLVMModuleSet()->dumpModulesToFile(".svf.bc");
//     LLVMModuleSet::releaseLLVMModuleSet();
//     llvm::llvm_shutdown();
//     return 0;
// }


#include "SVF-LLVM/SVFIRBuilder.h"
#include "SVF-LLVM/LLVMUtil.h"
#include "WPA/Andersen.h"
#include "WPA/WPAFSSolver.h"
#include "WPA/VersionedFlowSensitive.h"
#include "WPA/TypeAnalysis.h"
#include "WPA/AndersenPWC.h"

#include <regex>
#include <string>

using namespace SVF;


std::string InstructionsFileLocation = "";


std::unordered_set<std::string> instructionSet(std::string addrfile) {
    std::unordered_set<std::string> uniqueInstructions;
    std::ifstream infile(addrfile);

    if (!infile.is_open()) {
        // Handle file open error, could also throw an exception
        std::cerr << "Error: Could not open file " << addrfile << std::endl;
        return uniqueInstructions;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty()) {
            std::istringstream stream(line);
            std::string load1, load2, store1, store2;
            stream >> load1 >> load2 >> store1 >> store2;
            uniqueInstructions.insert(load1 + " " + load2);
            uniqueInstructions.insert(store1 + " " + store2);
        }
    }

    infile.close();
    return uniqueInstructions;
}

std::unordered_set<std::string> instructionPair(std::string addrfile) {
    std::unordered_set<std::string> uniqueInstructions;
    std::ifstream infile(addrfile);

    if (!infile.is_open()) {
        // Handle file open error, could also throw an exception
        std::cerr << "Error: Could not open file " << addrfile << std::endl;
        return uniqueInstructions;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty()) {
            uniqueInstructions.insert(line);
        }
    }

    infile.close();
    return uniqueInstructions;
}

std::string extractLocationInfo(const std::string& input) {
    std::regex dbgRegex(R"("ln":\s*(\d+),\s*"cl":\s*(\d+),\s*"fl":\s*"([^"]+))");
    std::smatch match;

    if (std::regex_search(input, match, dbgRegex)) {
        std::string line = match[1];
        std::string column = match[2];
        std::string file = match[3];
        return file + " " + line + ":" + column;
    }
    return "";
}

// Cleanup this function lol
std::string extractFileName(const AssignStmt* stmt) {
    std::string input = stmt->getValue()->toString();
    std::regex dbgRegex(R"("ln":\s*(\d+),\s*"cl":\s*(\d+),\s*"fl":\s*"([^"]+))");
    std::smatch match;

    if (std::regex_search(input, match, dbgRegex)) {
        std::string line = match[1];
        std::string column = match[2];
        std::string file = match[3];
        return file;
    }
    return "";
}

std::string findInstructionAddr(const AssignStmt* stmt, bool withFileLoc = true){
    std::string addr = extractLocationInfo(stmt->getValue()->toString());
    // if (auto inst = dyn_cast<llvm::Instruction>(stmt->getValue())) {
    //     // Retrieve the debug location
    //     llvm::DebugLoc debugLoc = inst->getDebugLoc();
        
    //     // Print the debug location information
    //     if (debugLoc) {
    //         addr = debugLoc.get()->getScope()->getFile()->getDirectory().str() + debugLoc.get()->getScope()->getFilename().str() + " " + std::to_string(debugLoc.getLine()) + ":" + std::to_string(debugLoc.getCol());
    //     } else {
    //         llvm::outs() << "No debug location available\n";
    //     }
    // }
    if (withFileLoc){
        addr = InstructionsFileLocation + addr;
    }
    return addr;
}

void runAliasAnalysis(PointerAnalysis* pta, const std::vector<const LoadStmt*>& loadInsts, const std::vector<const StoreStmt*>& storeInsts, const std::string& analysisName) {
    llvm::outs() << "\n=== Running Alias Analysis: " << analysisName << " ===\n";
    int countMay = 0;
    int countNo = 0;
    int countMust = 0;
    for (const LoadStmt* load : loadInsts) {
        // Get the corresponding LLVM instruction
        // if (const llvm::Instruction* inst = dyn_cast<llvm::Instruction>(load->getValue())) {
        //     // Retrieve the debug location
        //     llvm::DebugLoc debugLoc = inst->getDebugLoc();
            
        //     // Print the debug location information
        //     if (debugLoc) {
        //         llvm::outs() << "File: " << debugLoc.get()->getScope()->getFilename() 
        //                     << ", Line " << debugLoc.getLine() + ":" + debugLoc.getCol() << "\n";
        //     } else {
        //         llvm::outs() << "No debug location available\n";
        //     }
        // }
        // llvm::outs() << "SourceLoc: " << load->getValue()->getSourceLoc() << "\n";
        std::string loadfilename = extractFileName(load);
        for (const StoreStmt* store : storeInsts) {
            if (extractFileName(store)!=loadfilename){
                continue;
            }
            SVF::AliasResult result = pta->alias(load->getRHSVarID(), store->getLHSVarID());
            std::string aliasStr = "Unknown";
            switch (result) {
                case SVF::NoAlias: aliasStr = "NoAlias"; countNo++ ;break;
                case SVF::MayAlias: aliasStr = "MayAlias"; countMay++ ;break;
                case SVF::MustAlias: aliasStr = "MustAlias"; countMust++ ;break;
                default: llvm::outs() << "Should not get here!\n"; break;
            }
        }
    }
    llvm::outs() << "Total MustAlias Count = "<< countMust <<"\n";
    llvm::outs() << "Total MayAlias Count = "<< countMay <<"\n";
    llvm::outs() << "Total NoAlias Count = "<< countNo <<"\n";
}


void runAliasAnalysis(PointerAnalysis* pta, const std::vector<const LoadStmt*>& loadInsts, const std::vector<const StoreStmt*>& storeInsts, const std::unordered_set<std::string>& InstructionPair, const std::string& analysisName, std::string ResultFile) {
    llvm::outs() << "\n=== Running Alias Analysis: " << analysisName << " ===\n";
    
    std::ofstream outFile(ResultFile);

    // Check if the file opened successfully
    if (!outFile) {
        std::cerr << "Error opening the file for writing!" << std::endl;
        return;
    }
    
    #include <unordered_map>
    #include <string>
    std::unordered_map<std::string, int> output_buffer;
    for (const LoadStmt* load : loadInsts) {
        std::string loadS = findInstructionAddr(load, false);
        std::string loadfilename = extractFileName(load);
        for (const StoreStmt* store : storeInsts) {
            if (extractFileName(store)!=loadfilename) continue;
            std::string storeS = findInstructionAddr(store, false);
            if (InstructionPair.find((loadS + " " + storeS))==InstructionPair.end()) continue;
            
            SVF::AliasResult result = pta->alias(load->getRHSVarID(), store->getLHSVarID());
            switch (result) {
                case SVF::NoAlias: 
                    //outFile << "0 "<< loadS << " " << storeS << "\n";
                    output_buffer[loadS + " " + storeS] = std::max(0, output_buffer[loadS + " " + storeS]);
                    //llvm::outs() << "No!\n"; 
                    break;
                case SVF::MayAlias:
                    //outFile << "1 "<< loadS << " " << storeS << "\n";
                    output_buffer[loadS + " " + storeS] = std::max(1, output_buffer[loadS + " " + storeS]);
                    //llvm::outs() << "May!\n"; 
                    break;
                case SVF::MustAlias:
                    //outFile << "2 "<< loadS << " " << storeS << "\n";
                    output_buffer[loadS + " " + storeS] = 2;
                    //llvm::outs() << "Must!\n";
                    break;
                default: llvm::outs() << "Should not get here!\n"; break;
            }
        }
    }
    for (const auto &entry : output_buffer) {
      outFile << entry.second << " " << entry.first << "\n";
    }
    
    outFile.close();
    llvm::outs() << "Done\n";
}

void runAliasAnalysis(
    PointerAnalysis* pta,
    const std::vector<std::pair<const LoadStmt*, const StoreStmt*>>& loadStorePairs,
    const std::string& analysisName,
    const std::string& ResultFile
) {
    llvm::outs() << "\n=== Running Alias Analysis: " << analysisName << " ===\n";

    std::ofstream outFile(ResultFile);
    if (!outFile) {
        std::cerr << "Error opening the file for writing!" << std::endl;
        return;
    }
    
    std::unordered_map<std::string, int> output_buffer;


    llvm::outs() << "Total Comparisons = " << loadStorePairs.size() << "\n";
    for (const auto& pair : loadStorePairs) {
        const LoadStmt* load = pair.first;
        const StoreStmt* store = pair.second;

        std::string loadS = findInstructionAddr(load, false);
        std::string storeS = findInstructionAddr(store, false);
        std::string key = loadS + " " + storeS;

        SVF::AliasResult result = pta->alias(load->getRHSVarID(), store->getLHSVarID());
        switch (result) {
            case SVF::NoAlias:
                //llvm::outs() << "NoAlias\n";
                output_buffer[key] = std::max(0, output_buffer[key]);
                break;
            case SVF::MayAlias:
                //llvm::outs() << "MayAlias\n";
                output_buffer[key] = std::max(1, output_buffer[key]);
                break;
            case SVF::MustAlias:
                //llvm::outs() << "MustAlias\n";
                output_buffer[key] = 2;
                break;
            default:
                llvm::outs() << "Unexpected alias result\n";
                break;
        }
    }
    
    llvm::outs() << "Total Results = " << output_buffer.size() << "\n";
    for (const auto& entry : output_buffer) {
        outFile << entry.second << " " << entry.first << "\n";
    }
    
    outFile.close();
    llvm::outs() << "Done\n";
}


int main(int argc, char *argv[]) {

    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <LLVM Module> <Address File> <Result File> <Benchmark Name>" << std::endl;
        return 1;
    }

    std::string llvmModule = argv[1];
    std::string addrfile = argv[2];
    std::string resultfile = argv[3];
    std::string benchmark = argv[4];

    InstructionsFileLocation = "/work/johan/spec2017/benchspec/CPU/" + benchmark + "/build/build_peak_instrumentedforPGO-64.0000";

    std::vector<std::string> moduleNameVec = {llvmModule};

    if (Options::WriteAnder() == "ir_annotator") {
        LLVMModuleSet::preProcessBCs(moduleNameVec);
    }

    LLVMModuleSet::buildSVFModule(moduleNameVec);
    
    // Build SVF IR
    SVFIRBuilder builder;
    SVFIR* pag = builder.build();
    

    // === STEP 1: Collect Load and Store instructions
    std::vector<const LoadStmt*> loadInsts;
    std::vector<const StoreStmt*> storeInsts;
    std::unordered_set<std::string> InstructionToCheck = instructionSet(addrfile);
    std::unordered_set<std::string> InstructionPair = instructionPair(addrfile);



    ICFG* icfg = pag->getICFG();
    for (auto it = icfg->begin(); it != icfg->end(); ++it) {
        const ICFGNode* icfgNode = it->second;
        if (!pag->hasSVFStmtList(icfgNode))
            continue;

        SVFIR::SVFStmtList& stmtList = pag->getSVFStmtList(icfgNode);
        for (const SVFStmt* stmt : stmtList) {
            const LoadStmt* load = SVFUtil::dyn_cast<LoadStmt>(stmt);
            //if (load) llvm::outs() << findInstructionAddr(load) << "\n";
            if (load && (InstructionToCheck.find(findInstructionAddr(load, false))!=InstructionToCheck.end())) {
                //llvm::outs() << "FOUNDD!!" << "\n";
                loadInsts.push_back(load);
                continue;
            }
            const StoreStmt* store = SVFUtil::dyn_cast<StoreStmt>(stmt);
            //if (store) llvm::outs() << findInstructionAddr(store) << "\n";
            if (store && (InstructionToCheck.find(findInstructionAddr(store, false))!=InstructionToCheck.end())) {
                storeInsts.push_back(store);
                continue;
            }
        }
    }
    
    
    //llvm::outs() << "Constructing pairs\n";
    //std::vector<std::pair<const LoadStmt*, const StoreStmt*>> loadStorePairs;
    // Cache the Load and Store instructions found in this node
    //std::vector<const LoadStmt*> localLoads;
    //std::vector<const StoreStmt*> localStores;
    
    //ICFG* icfg = pag->getICFG();
    //for (auto it = icfg->begin(); it != icfg->end(); ++it) {
    //    const ICFGNode* icfgNode = it->second;
    //    if (!pag->hasSVFStmtList(icfgNode))
    //        continue;
    
    //    SVFIR::SVFStmtList& stmtList = pag->getSVFStmtList(icfgNode);
    
    
    //    for (const SVFStmt* stmt : stmtList) {
    //        const LoadStmt* load = SVFUtil::dyn_cast<LoadStmt>(stmt);
    //        if (load) {
    //            localLoads.push_back(load);
    //        }
    
    //        const StoreStmt* store = SVFUtil::dyn_cast<StoreStmt>(stmt);
    //        if (store) {
    //            localStores.push_back(store);
    //        }
    //    }
    
    //}
    
    
    
    // Cross-match local load/store pairs for this basic block
    //for (const LoadStmt* load : localLoads) {
    //    std::string loadAddr = findInstructionAddr(load, false);

    //    for (const StoreStmt* store : localStores) {
    //        std::string storeAddr = findInstructionAddr(store, false);
    //        std::string pairKey = loadAddr + " " + storeAddr;

    //        if (InstructionPair.find(pairKey) != InstructionPair.end()) {
    //            loadStorePairs.push_back({load, store});
    //        }
    //    }
    //}
    
    //llvm::outs() << "Constructed pairs\n";
    //llvm::outs() << "Starting analysis\n";

    // === STEP 2: Run each pointer analysis

     // Andersen
     {
        AndersenSFR* ander = AndersenSFR::createAndersenSFR(pag);
        runAliasAnalysis(ander, loadInsts, storeInsts, InstructionPair , "AndersenSFR", resultfile);
        AndersenSFR::releaseAndersenSFR();
     }
    
    // Andersen
    // {
    //     Andersen* ander = AndersenWaveDiff::createAndersenWaveDiff(pag);
    //     runAliasAnalysis(ander, loadInsts, storeInsts, "AndersenWaveDiff");
    //     AndersenWaveDiff::releaseAndersenWaveDiff();
    // }

    // // FSSPARSE_WPA (Field-sensitive flow-insensitive analysis)
    // {
    //     FSSPARSE* fspta = FSSPARSE::createFSSPARSE(pag);
    //     runAliasAnalysis(fspta, loadInsts, storeInsts, "FSSPARSE_WPA");
    //     FSSPARSE::releaseFSSPARSE();
    // }

    // VFS_WPA (Value-flow-sensitive analysis)
    // {
    //     VersionedFlowSensitive* vfspta = VersionedFlowSensitive::createVFSWPA(pag);
    //     runAliasAnalysis(vfspta, loadInsts, storeInsts, "VFS_WPA", resultfile);
    //     VersionedFlowSensitive::releaseVFSWPA();
    // }

    // // TypeCPP_WPA (Type-based pointer analysis)
    // {
    //     TypeBasedPointerAnalysis* typepta = TypeBasedPointerAnalysis::createTypeBasedPointerAnalysis(pag);
    //     runAliasAnalysis(typepta, loadInsts, storeInsts, "TypeCPP_WPA");
    //     TypeBasedPointerAnalysis::releaseTypeBasedPointerAnalysis();
    // }

    // === Cleanup
    SVFIR::releaseSVFIR();
    // LLVMModuleSet::getLLVMModuleSet()->dumpModulesToFile(".svf.bc");
    LLVMModuleSet::releaseLLVMModuleSet();
    llvm::llvm_shutdown();
    return 0;
}