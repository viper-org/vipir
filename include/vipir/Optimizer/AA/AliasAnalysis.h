// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_AA_ALIAS_ANALYSIS_H
#define VIPIR_OPTIMIZER_AA_ALIAS_ANALYSIS_H 1

#include "vipir/Pass/Pass.h"

namespace vipir
{
    class Function;
    class Value;

    namespace opt
    {
        class AliasAnalyzer
        {
        struct Node
        {
            Node(Value* value, std::vector<Node*> children);
            Value* value;
            std::vector<Node*> children;
        };
        public:
            void buildGraph(Function* function);
            void printGraph();

        private:
            std::vector<Node*> mGraph;
            std::vector<std::unique_ptr<Node> > mNodes;

            void printOne(Node* node, std::ostream& stream);
        };

        class AAPass : public Pass
        {
        public:
            AAPass();

            virtual void execute(Module& module) override;
        };
    }
}

#endif // VIPIR_OPTIMIZER_AA_ALIAS_ANALYSIS_H