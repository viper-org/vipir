// Copyright 2024 solar-mist

#include "vipir/Optimizer/AA/AliasAnalysis.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/Module.h"

#include <algorithm>
#include <fstream>

namespace vipir
{
    namespace opt
    {
        AliasAnalyzer::Node::Node(Value* value, std::vector<Node*> children)
            : value(value)
            , children(std::move(children))
        {
        }

        void AliasAnalyzer::buildGraph(Function* function)
        {
            for (auto& basicBlock : function->mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    // a = &b
                    if (auto addr = dynamic_cast<AddrInst*>(value.get()))
                    {
                        auto ptr = addr->mPtr;

                        auto childNode = std::make_unique<Node>(ptr, std::vector<Node*>());
                        auto parentNode = std::make_unique<Node>(addr, std::vector<Node*>{childNode.get()});
                        mGraph.push_back(parentNode.get());

                        mNodes.push_back(std::move(childNode));
                        mNodes.push_back(std::move(parentNode));
                    }
                    // a = *b
                    else if (auto load = dynamic_cast<LoadInst*>(value.get()))
                    {
                        auto ptr = load->mPtr;

                        auto childNode = std::make_unique<Node>(load, std::vector<Node*>());
                        auto parentNode = std::make_unique<Node>(ptr, std::vector<Node*>{childNode.get()});
                        mGraph.push_back(parentNode.get());

                        mNodes.push_back(std::move(childNode));
                        mNodes.push_back(std::move(parentNode));
                    }
                }
            }
        }

        void AliasAnalyzer::printGraph()
        {
            std::ofstream graphout("vipir.dot");
            graphout<< "\n\ndigraph {";
            for(auto node : mGraph)
            {
                printOne(node, graphout);
            }
            graphout << "\n}";
        }

        void AliasAnalyzer::printOne(Node* node, std::ostream& stream)
        {
            auto name = node->value->ident();
            auto it = std::find_if(name.rbegin(), name.rend(), [](char c){ return !isalnum(c); });
            name.erase(name.rend().base(), it.base());

            for (auto child : node->children)
            {
                auto childName = child->value->ident();
                auto it = std::find_if(childName.rbegin(), childName.rend(), [](char c){ return !isalnum(c); });
                childName.erase(childName.rend().base(), it.base());

                stream << "\n\tv" << name << " -> v" << childName;
                printOne(child, stream);
            }
        }


        AAPass::AAPass()
            : Pass(PassType::AliasAnalysis)
        {
        }

        void AAPass::execute(Module& module)
        {
            AliasAnalyzer aa;
            for (const auto& global : module.getGlobals())
            {
                if (auto function = dynamic_cast<Function*>(global.get()))
                {
                    aa.buildGraph(function);
                }
            }
            aa.printGraph();
        }
    }
}