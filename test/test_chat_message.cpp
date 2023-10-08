#include <gtest/gtest.h>
#include "chat_message.h"

// Test case for serializing and deserializing a ChatMessage
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

// Test case for handling different message types
TEST(ChatMessageTest, MessageTypeHandling) {
    // Create a command message
    ChatMessage cmdMsg;
    cmdMsg.type = MSG_COMMAND;
    cmdMsg.from = "Admin";
    cmdMsg.to = "Bob";
    cmdMsg.message = "Shutdown server";

    // Create a file name message
    ChatMessage fileMsg;
    fileMsg.type = MSG_FILEHEADER;
    fileMsg.from = "Alice";
    fileMsg.to = "Bob";
    fileMsg.message = "document.txt:12321";

    std::string serializedCmdMsg = SerializeChatMessage(cmdMsg);
    std::string serializedFileMsg = SerializeChatMessage(fileMsg);

    // Deserialize the string back to a ChatMessage
    ChatMessage deserializedCmdMsg = DeserializeChatMessage(serializedCmdMsg);
    // Deserialize the string back to a ChatMessage
    ChatMessage deserializedFileMsg = DeserializeChatMessage(serializedFileMsg);

    // Check if the deserialized cmd message matches the original
    EXPECT_EQ(cmdMsg.type, deserializedCmdMsg.type);
    EXPECT_EQ(cmdMsg.from, deserializedCmdMsg.from);
    EXPECT_EQ(cmdMsg.to, deserializedCmdMsg.to);
    EXPECT_EQ(cmdMsg.message, deserializedCmdMsg.message);

    // Check if the deserialized file message matches the original
    EXPECT_EQ(fileMsg.type, deserializedFileMsg.type);
    EXPECT_EQ(fileMsg.from, deserializedFileMsg.from);
    EXPECT_EQ(fileMsg.to, deserializedFileMsg.to);
    EXPECT_EQ(fileMsg.message, deserializedFileMsg.message);
}

// Test case for handling empty messages
TEST(ChatMessageTest, EmptyMessageHandling) {
    // Create an empty message
    ChatMessage emptyMsg;
    emptyMsg.type = MSG_TEXT;
    emptyMsg.from = "Alice";
    emptyMsg.to = "Bob";
    // message is empty

    // Serialize the empty message and then deserialize it
    std::string serialized = SerializeChatMessage(emptyMsg);
    ChatMessage deserializedMsg = DeserializeChatMessage(serialized);

    // Check if the deserialized message matches the original
    EXPECT_EQ(emptyMsg.type, deserializedMsg.type);
    EXPECT_EQ(emptyMsg.from, deserializedMsg.from);
    EXPECT_EQ(emptyMsg.to, deserializedMsg.to);
    EXPECT_EQ(emptyMsg.message, deserializedMsg.message);
}

// Test case for handling empty "to" field
TEST(ChatMessageTest, EmptyToFieldHandling) {
    // Create an empty "to" field
    ChatMessage emptyToMsg;
    emptyToMsg.type = MSG_TEXT;
    emptyToMsg.from = "Alice";
    emptyToMsg.to = "";  // "to" field is empty
    emptyToMsg.message = "Hello, Bob!";

    // Serialize the message and then deserialize it
    std::string serialized = SerializeChatMessage(emptyToMsg);
    ChatMessage deserializedMsg = DeserializeChatMessage(serialized);

    // Check if the deserialized message matches the original
    EXPECT_EQ(emptyToMsg.type, deserializedMsg.type);
    EXPECT_EQ(emptyToMsg.from, deserializedMsg.from);
    EXPECT_EQ(emptyToMsg.to, deserializedMsg.to);
    EXPECT_EQ(emptyToMsg.message, deserializedMsg.message);
}

// Test case for handling empty "from" field
TEST(ChatMessageTest, EmptyFromFieldHandling) {
    // Create an empty "from" field
    ChatMessage emptyFromMsg;
    emptyFromMsg.type = MSG_TEXT;
    emptyFromMsg.from = "";  // "from" field is empty
    emptyFromMsg.to = "Bob";
    emptyFromMsg.message = "Hello, Bob!";

    // Serialize the message and then deserialize it
    std::string serialized = SerializeChatMessage(emptyFromMsg);
    ChatMessage deserializedMsg = DeserializeChatMessage(serialized);

    // Check if the deserialized message matches the original
    EXPECT_EQ(emptyFromMsg.type, deserializedMsg.type);
    EXPECT_EQ(emptyFromMsg.from, deserializedMsg.from);
    EXPECT_EQ(emptyFromMsg.to, deserializedMsg.to);
    EXPECT_EQ(emptyFromMsg.message, deserializedMsg.message);
}

// Test case for handling unset "to" field
TEST(ChatMessageTest, UnsetToFieldHandling) {
    // Create a message with unset "to" field
    ChatMessage unsetToMsg;
    unsetToMsg.type = MSG_TEXT;
    unsetToMsg.from = "Alice";
    // "to" field is not set
    unsetToMsg.message = "Hello, Bob!";

    // Serialize the message and then deserialize it
    std::string serialized = SerializeChatMessage(unsetToMsg);
    ChatMessage deserializedMsg = DeserializeChatMessage(serialized);

    // Check if the deserialized message matches the original
    EXPECT_EQ(unsetToMsg.type, deserializedMsg.type);
    EXPECT_EQ(unsetToMsg.from, deserializedMsg.from);
    EXPECT_EQ(unsetToMsg.to, deserializedMsg.to);
    EXPECT_EQ(unsetToMsg.message, deserializedMsg.message);
}

// Test case for handling unset "from" field
TEST(ChatMessageTest, UnsetFromFieldHandling) {
    // Create a message with unset "from" field
    ChatMessage unsetFromMsg;
    unsetFromMsg.type = MSG_TEXT;
    // "from" field is not set
    unsetFromMsg.to = "Bob";
    unsetFromMsg.message = "Hello, Bob!";

    // Serialize the message and then deserialize it
    std::string serialized = SerializeChatMessage(unsetFromMsg);
    ChatMessage deserializedMsg = DeserializeChatMessage(serialized);

    // Check if the deserialized message matches the original
    EXPECT_EQ(unsetFromMsg.type, deserializedMsg.type);
    EXPECT_EQ(unsetFromMsg.from, deserializedMsg.from);
    EXPECT_EQ(unsetFromMsg.to, deserializedMsg.to);
    EXPECT_EQ(unsetFromMsg.message, deserializedMsg.message);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
