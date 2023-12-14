#include "pch.h"
#include "CppUnitTest.h"
#include "../CA2/XMLParser.h"
#include "../CA2/TreeNode.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XMLParserTest
{
    TEST_CLASS(TestXMLParser)
    {
    public:
        TEST_METHOD(TestParseXML)
        {
            XMLParser parser("valid.xml"); // Replace with the path to your actual XML file
            TreeNode* root = parser.parseXML();

            Assert::IsNotNull(root); // Verify that the root is not null, indicating successful parsing
            // Additional checks can be added to validate the structure of the tree
        }

        TEST_METHOD(TestValidateXML_ValidFile)
        {
            XMLParser validParser("valid.xml");
            Assert::IsTrue(validParser.validateXML());
        }

        TEST_METHOD(TestValidateXML_InvalidFile)
        {
            XMLParser invalidParser("invalid.xml"); // Use an invalid XML file path
            Assert::IsFalse(invalidParser.validateXML());
        }
    };
}
