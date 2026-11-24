#include "pch.h"
#include "BitField.h"
#include "Set.h"

TEST(BitFieldTest, DefaultConstructor) {
    BitField field;
    EXPECT_EQ(field.get_size(), 0);
}

TEST(BitFieldTest, SizeConstructor) {
    BitField field(10);
    EXPECT_EQ(field.get_size(), 10);
}

TEST(BitFieldTest, SetBit) {
    BitField field(10);
    field.set_bit(3);
    EXPECT_TRUE(field.get_bit(3));
}

TEST(BitFieldTest, ResetBit) {
    BitField field(10);
    field.set_bit(3);
    field.reset_bit(3);
    EXPECT_FALSE(field.get_bit(3));
}

TEST(BitFieldTest, GetBit) {
    BitField field(10);
    field.set_bit(5);
    EXPECT_TRUE(field.get_bit(5));
    EXPECT_FALSE(field.get_bit(4));
}

TEST(BitFieldTest, OutOfRange) {
    BitField field(10);
    EXPECT_THROW(field.set_bit(10), std::out_of_range);
    EXPECT_THROW(field.get_bit(10), std::out_of_range);
    EXPECT_THROW(field.reset_bit(10), std::out_of_range);
}

TEST(BitFieldTest, CopyConstructor) {
    BitField field(10);
    field.set_bit(3);
    BitField copy(field);
    EXPECT_EQ(copy.get_size(), 10);
    EXPECT_TRUE(copy.get_bit(3));
}

TEST(BitFieldTest, CopyAssignment) {
    BitField first(10);
    first.set_bit(5);
    BitField second;
    second = first;
    EXPECT_EQ(second.get_size(), 10);
    EXPECT_TRUE(second.get_bit(5));
}

TEST(BitFieldTest, MoveConstructor) {
    BitField first(10);
    first.set_bit(4);
    BitField second(std::move(first));
    EXPECT_EQ(second.get_size(), 10);
    EXPECT_TRUE(second.get_bit(4));
}

TEST(BitFieldTest, BitwiseAnd) {
    BitField first(5);
    BitField second(5);
    first.set_bit(1);
    first.set_bit(2);
    second.set_bit(2);
    second.set_bit(3);
    BitField result = first & second;
    EXPECT_FALSE(result.get_bit(1));
    EXPECT_TRUE(result.get_bit(2));
    EXPECT_FALSE(result.get_bit(3));
}

TEST(BitFieldTest, BitwiseOr) {
    BitField first(5);
    BitField second(5);
    first.set_bit(1);
    second.set_bit(2);
    BitField result = first | second;
    EXPECT_TRUE(result.get_bit(1));
    EXPECT_TRUE(result.get_bit(2));
}

TEST(BitFieldTest, BitwiseNot) {
    BitField field(5);
    field.set_bit(1);
    BitField result = ~field;
    EXPECT_TRUE(result.get_bit(0));
    EXPECT_FALSE(result.get_bit(1));
    EXPECT_TRUE(result.get_bit(2));
}

TEST(BitFieldTest, AndAssignment) {
    BitField first(5);
    BitField second(5);
    first.set_bit(1);
    first.set_bit(2);
    second.set_bit(2);
    first &= second;
    EXPECT_FALSE(first.get_bit(1));
    EXPECT_TRUE(first.get_bit(2));
}

TEST(BitFieldTest, OrAssignment) {
    BitField first(5);
    BitField second(5);
    first.set_bit(1);
    second.set_bit(2);
    first |= second;
    EXPECT_TRUE(first.get_bit(1));
    EXPECT_TRUE(first.get_bit(2));
}

TEST(BitFieldTest, Output) {
    BitField field(5);
    field.set_bit(0);
    field.set_bit(2);
    field.set_bit(4);
    std::stringstream stream;
    stream << field;
    EXPECT_EQ(stream.str(), "10101");
}

TEST(BitFieldTest, Input) {
    BitField field;
    std::stringstream stream("10101");
    stream >> field;
    EXPECT_EQ(field.get_size(), 5);
    EXPECT_TRUE(field.get_bit(0));
    EXPECT_FALSE(field.get_bit(1));
    EXPECT_TRUE(field.get_bit(2));
    EXPECT_FALSE(field.get_bit(3));
    EXPECT_TRUE(field.get_bit(4));
}

TEST(BitFieldTest, InvalidInput) {
    BitField field;
    std::stringstream stream("10201");
    stream >> field;
    EXPECT_TRUE(stream.fail());
}

TEST(SetTest, DefaultConstructor) {
    Set set;
    EXPECT_FALSE(set * 0);
}

TEST(SetTest, SizeConstructor) {
    Set set(10);
    EXPECT_FALSE(set * 5);
}

TEST(SetTest, Insert) {
    Set set(10);
    set.insert(5);
    EXPECT_TRUE(set.is_member(5));
}

TEST(SetTest, Erase) {
    Set set(10);
    set.insert(5);
    set.erase(5);
    EXPECT_FALSE(set.is_member(5));
}

TEST(SetTest, IsMember) {
    Set set(10);
    set.insert(3);
    EXPECT_TRUE(set.is_member(3));
    EXPECT_FALSE(set.is_member(4));
}

TEST(SetTest, AddElement) {
    Set set(5);
    set.insert(1);
    Set result = set + 3;
    EXPECT_TRUE(result.is_member(1));
    EXPECT_TRUE(result.is_member(3));
}

TEST(SetTest, RemoveElement) {
    Set set(5);
    set.insert(1);
    set.insert(3);
    Set result = set - 1;
    EXPECT_FALSE(result.is_member(1));
    EXPECT_TRUE(result.is_member(3));
}

TEST(SetTest, ContainsElement) {
    Set set(5);
    set.insert(2);
    EXPECT_TRUE(set * 2);
    EXPECT_FALSE(set * 3);
}

TEST(SetTest, Union) {
    Set first(10);
    Set second(10);
    first.insert(1);
    first.insert(2);
    second.insert(2);
    second.insert(3);
    Set result = first + second;
    EXPECT_TRUE(result.is_member(1));
    EXPECT_TRUE(result.is_member(2));
    EXPECT_TRUE(result.is_member(3));
}

TEST(SetTest, Difference) {
    Set first(10);
    Set second(10);
    first.insert(1);
    first.insert(2);
    second.insert(2);
    second.insert(3);
    Set result = first - second;
    EXPECT_TRUE(result.is_member(1));
    EXPECT_FALSE(result.is_member(2));
}

TEST(SetTest, Intersection) {
    Set first(10);
    Set second(10);
    first.insert(1);
    first.insert(2);
    second.insert(2);
    second.insert(3);
    Set result = first * second;
    EXPECT_FALSE(result.is_member(1));
    EXPECT_TRUE(result.is_member(2));
    EXPECT_FALSE(result.is_member(3));
}

TEST(SetTest, Complement) {
    Set set(5);
    set.insert(1);
    set.insert(3);
    Set result = -set;
    EXPECT_TRUE(result.is_member(0));
    EXPECT_FALSE(result.is_member(1));
    EXPECT_TRUE(result.is_member(2));
    EXPECT_FALSE(result.is_member(3));
    EXPECT_TRUE(result.is_member(4));
}

TEST(SetTest, AddAssignment) {
    Set set(5);
    set += 2;
    EXPECT_TRUE(set.is_member(2));
}

TEST(SetTest, RemoveAssignment) {
    Set set(5);
    set.insert(2);
    set -= 2;
    EXPECT_FALSE(set.is_member(2));
}

TEST(SetTest, UnionAssignment) {
    Set first(5);
    Set second(5);
    first.insert(1);
    second.insert(2);
    first += second;
    EXPECT_TRUE(first.is_member(1));
    EXPECT_TRUE(first.is_member(2));
}

TEST(SetTest, DifferenceAssignment) {
    Set first(5);
    Set second(5);
    first.insert(1);
    first.insert(2);
    second.insert(2);
    first -= second;
    EXPECT_TRUE(first.is_member(1));
    EXPECT_FALSE(first.is_member(2));
}

TEST(SetTest, IntersectionAssignment) {
    Set first(5);
    Set second(5);
    first.insert(1);
    first.insert(2);
    second.insert(2);
    first *= second;
    EXPECT_FALSE(first.is_member(1));
    EXPECT_TRUE(first.is_member(2));
}

TEST(SetTest, CopyConstructor) {
    Set first(5);
    first.insert(2);
    Set second(first);
    EXPECT_TRUE(second.is_member(2));
}

TEST(SetTest, CopyAssignment) {
    Set first(5);
    first.insert(3);
    Set second;
    second = first;
    EXPECT_TRUE(second.is_member(3));
}

TEST(SetTest, ConvertToBitField) {
    Set set(5);
    set.insert(2);
    BitField field = static_cast<BitField>(set);
    EXPECT_TRUE(field.get_bit(2));
}

TEST(SetTest, Output) {
    Set set(10);
    set.insert(2);
    set.insert(4);
    set.insert(6);
    std::stringstream stream;
    stream << set;
    EXPECT_EQ(stream.str(), "{ 2, 4, 6 }");
}

TEST(SetTest, Input) {
    Set set;
    std::stringstream stream("{ 2, 3, 5, 7 }");
    stream >> set;
    EXPECT_TRUE(set.is_member(2));
    EXPECT_TRUE(set.is_member(3));
    EXPECT_TRUE(set.is_member(5));
    EXPECT_TRUE(set.is_member(7));
    EXPECT_FALSE(set.is_member(4));
}

TEST(SetTest, EmptySetInput) {
    Set set;
    std::stringstream stream("{ }");
    stream >> set;
    EXPECT_EQ(static_cast<BitField>(set).get_size(), 0);
}