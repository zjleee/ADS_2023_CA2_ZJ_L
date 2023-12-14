#include "pch.h"
#include "CppUnitTest.h"
#include "../CA2/TreeNode.h"
#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TreeNodeTest
{
    TEST_CLASS(TestTreeNode)
    {
    public:
        TEST_METHOD(TestConstructorAndAddChild)
        {
            TreeNode root("root", "dir");
            TreeNode* child = new TreeNode("child", "dir");
            root.addChild(child);

            Assert::IsNotNull(root.findNode("child"));
        }

        TEST_METHOD(TestCountItems)
        {
            TreeNode root("root", "dir");
            root.addChild(new TreeNode("child1", "dir"));
            root.addChild(new TreeNode("child2", "file", "100"));

            Assert::AreEqual(2, root.countItems());
        }

        TEST_METHOD(TestMemoryUsed)
        {
            TreeNode root("root", "dir");
            root.addChild(new TreeNode("file1", "file", "100"));
            root.addChild(new TreeNode("file2", "file", "200"));

            Assert::AreEqual(300, root.memoryUsed());
        }
        
        TEST_METHOD(TestFindNode)
        {
            TreeNode root("root", "dir");
            root.addChild(new TreeNode("child", "dir"));

            Assert::IsNotNull(root.findNode("child"));
            Assert::IsNull(root.findNode("nonexistent"));
        }

        TEST_METHOD(TestPruneEmptyFolders)
        {
            TreeNode root("root", "dir");
            root.addChild(new TreeNode("emptyDir1", "dir"));
            root.addChild(new TreeNode("emptyDir2", "dir"));

            int prunedCount = root.pruneEmptyFolders();
            Assert::AreEqual(2, prunedCount);
        }

        TEST_METHOD(TestFindPath)
        {
            TreeNode root("root", "dir");
            TreeNode* child1 = new TreeNode("child1", "dir");
            root.addChild(child1);
            child1->addChild(new TreeNode("child2", "file", "100"));

            Assert::AreEqual(std::string("/root/child1/child2"), root.findPath("child2"));
            Assert::AreEqual(std::string(""), root.findPath("nonexistent"));
        }
    };
}