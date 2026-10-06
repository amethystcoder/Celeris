//////////////////////////////////////////////////////
// Possible notes for testing.
// There might be the possibility that tests would access the AST instances directly, 
// rather than through the ASTManager
// //////////////////////////////////////////////////

#include "core/storeNode.h"
#include <gtest/gtest.h>

namespace Celeris {

    // ============================================================
    // ResolveInt
    // ============================================================

    TEST(ResolveIntTest, AcceptsPositiveIntegers)
    {
        EXPECT_TRUE(ResolveInt::check_type_correctness("0"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("1"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("123"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("123456789"));
    }

    TEST(ResolveIntTest, AcceptsNegativeIntegers)
    {
        EXPECT_TRUE(ResolveInt::check_type_correctness("-0"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("-1"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("-123"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("-123456789"));
    }

    TEST(ResolveIntTest, AcceptsExplicitPositiveSign)
    {
        EXPECT_TRUE(ResolveInt::check_type_correctness("+0"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("+1"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("+123"));
    }

    TEST(ResolveIntTest, RejectsEmptyString)
    {
        EXPECT_FALSE(ResolveInt::check_type_correctness(""));
    }

    TEST(ResolveIntTest, RejectsSignOnly)
    {
        EXPECT_FALSE(ResolveInt::check_type_correctness("+"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("-"));
    }

    TEST(ResolveIntTest, RejectsNonDigitCharacters)
    {
        EXPECT_FALSE(ResolveInt::check_type_correctness("123a"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("a123"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("12a34"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("12.34"));
    }

    TEST(ResolveIntTest, RejectsMultipleSigns)
    {
        EXPECT_FALSE(ResolveInt::check_type_correctness("++123"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("--123"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("+-123"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("-+123"));
    }

    TEST(ResolveIntTest, RejectsSignInMiddle)
    {
        EXPECT_FALSE(ResolveInt::check_type_correctness("12-34"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("12+34"));
    }

    TEST(ResolveIntTest, RejectsWhitespace)
    {
        EXPECT_FALSE(ResolveInt::check_type_correctness(" 123"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("123 "));
        EXPECT_FALSE(ResolveInt::check_type_correctness(" 123 "));
        EXPECT_FALSE(ResolveInt::check_type_correctness("+ 123"));
        EXPECT_FALSE(ResolveInt::check_type_correctness("- 123"));
    }

    TEST(ResolveIntTest, AcceptsLeadingZeros)
    {
        EXPECT_TRUE(ResolveInt::check_type_correctness("000"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("00123"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("-00123"));
        EXPECT_TRUE(ResolveInt::check_type_correctness("+00123"));
    }


    // ============================================================
    // ResolveString
    // ============================================================

    TEST(ResolveStringTest, EmptyString)
    {
        // eventually considers an empty string invalid.
        EXPECT_TRUE(ResolveString::check_type_correctness(""));
    }

    TEST(ResolveStringTest, NormalString)
    {
        EXPECT_TRUE(ResolveString::check_type_correctness("hello"));
        EXPECT_TRUE(ResolveString::check_type_correctness("hello world"));
        EXPECT_TRUE(ResolveString::check_type_correctness("123"));
    }


    // ============================================================
    // ResolveBoolean
    // ============================================================

    TEST(ResolveBooleanTest, AcceptsTrue)
    {
        EXPECT_TRUE(ResolveBoolean::check_type_correctness("true"));
    }

    TEST(ResolveBooleanTest, AcceptsFalse)
    {
        EXPECT_TRUE(ResolveBoolean::check_type_correctness("false"));
    }

    TEST(ResolveBooleanTest, RejectsInvalidValues)
    {
        EXPECT_FALSE(ResolveBoolean::check_type_correctness(""));
        EXPECT_FALSE(ResolveBoolean::check_type_correctness("123"));
        EXPECT_FALSE(ResolveBoolean::check_type_correctness("hello"));
    }

} // namespace Celeris