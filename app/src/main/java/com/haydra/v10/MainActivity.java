package com.haydra.v10;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.graphics.Color;
import android.view.Gravity;
import android.view.ViewGroup;

public class MainActivity extends Activity {

    private EditText messageInput;
    private Button sendButton;
    private LinearLayout chatContainer;
    private ScrollView chatScroll;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        messageInput = findViewById(R.id.messageInput);
        sendButton = findViewById(R.id.sendButton);
        chatContainer = findViewById(R.id.chatContainer);
        chatScroll = findViewById(R.id.chatScroll);

        sendButton.setOnClickListener(v -> sendMessage());

        messageInput.setOnEditorActionListener((v, actionId, event) -> {
            sendMessage();
            return true;
        });
    }

    private void sendMessage() {
        String message = messageInput.getText().toString().trim();

        if (message.isEmpty()) {
            return;
        }

        addMessage("👤 " + message, true);

        messageInput.setText("");

        String reply = "🐉 وصلتني رسالتك: " + message;

        addMessage(reply, false);

        chatScroll.post(() ->
                chatScroll.fullScroll(ScrollView.FOCUS_DOWN)
        );
    }

    private void addMessage(String text, boolean user) {
        TextView messageView = new TextView(this);

        messageView.setText(text);
        messageView.setTextColor(Color.WHITE);
        messageView.setTextSize(16);
        messageView.setPadding(14, 12, 14, 12);

        LinearLayout.LayoutParams params =
                new LinearLayout.LayoutParams(
                        ViewGroup.LayoutParams.WRAP_CONTENT,
                        ViewGroup.LayoutParams.WRAP_CONTENT
                );

        params.setMargins(0, 8, 0, 8);

        if (user) {
            params.gravity = Gravity.END;
            messageView.setBackgroundColor(Color.rgb(35, 75, 105));
        } else {
            params.gravity = Gravity.START;
            messageView.setBackgroundColor(Color.rgb(24, 33, 43));
        }

        chatContainer.addView(messageView, params);
    }
}
