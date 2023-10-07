#include <gtest/gtest.h>
#include "chat_message.h"

TEST(ChatMessageTest, SerializationDeserialization) {
    // Create a ChatMessage for testing
    ChatMessage originalMsg;
    originalMsg.type = MSG_TEXT;
    originalMsg.from = "Alice";
    originalMsg.to = "Bob";
    originalMsg.message = "Hello, Bob!";

    // Serialize the ChatMessage to a string
    std::string serialized = SerializeChatMessage(originalMsg);

    // Deserialize the string back to a ChatMessage
    ChatMessage deserializedMsg = DeserializeChatMessage(serialized);

    // Check if the deserialized message matches the original
    EXPECT_EQ(originalMsg.type, deserializedMsg.type);
    EXPECT_EQ(originalMsg.from, deserializedMsg.from);
    EXPECT_EQ(originalMsg.to, deserializedMsg.to);
    EXPECT_EQ(originalMsg.message, deserializedMsg.message);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
