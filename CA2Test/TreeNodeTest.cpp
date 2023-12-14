#include "pch.h"
#include "CppUnitTest.h"
#include "../CA2/TreeNode.h"  
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TreeNodeTest
{
	TEST_CLASS(TestTreeNode)
	{
	public:

		TEST_METHOD(TestCountItems)
		{
			TreeNode root();

			Assert::AreEqual(2, 2);
		}
	};
}