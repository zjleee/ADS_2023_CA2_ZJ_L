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
            XMLParser parser("valid.xml"); 
            TreeNode* root = parser.parseXML();

            Assert::IsNotNull(root); 
        }

        TEST_METHOD(TestValidateXML_ValidFile)
        {
            XMLParser validParser("valid.xml");
            Assert::IsTrue(validParser.validateXML());
        }

        TEST_METHOD(TestValidateXML_InvalidFile)
        {
            XMLParser invalidParser("invalid.xml"); 
            Assert::IsFalse(invalidParser.validateXML());
        }
    };
}
