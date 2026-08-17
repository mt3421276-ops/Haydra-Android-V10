package com.haydra.v10;

import android.app.Activity;
import android.os.Bundle;
import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class MainActivity extends Activity {

    private LinearLayout chatContainer;
    private EditText messageInput;
    private ScrollView chatScroll;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        chatContainer = findViewById(R.id.chatContainer);
        messageInput = findViewById(R.id.messageInput);
        chatScroll = findViewById(R.id.chatScroll);

        Button sendButton = findViewById(R.id.sendButton);

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

        addMessage("أنت:\n" + message, true);

        messageInput.setText("");

        // مؤقتًا، إلى أن نربط محرك Haydra الحقيقي.
        addMessage("🐉 Haydra:\nوصلتني رسالتك. سيتم ربط محرك Haydra في الخطوة التالية.", false);

        chatScroll.post(() ->
                chatScroll.fullScroll(View.FOCUS_DOWN)
        );
    }

    private void addMessage(String text, boolean user) {

        TextView messageView = new TextView(this);

        messageView.setText(text);
        messageView.setTextSize(16);
        messageView.setTextColor(Color.WHITE);
        messageView.setPadding(16, 14, 16, 14);

        LinearLayout.LayoutParams params =
                new LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.WRAP_CONTENT,
                        LinearLayout.LayoutParams.WRAP_CONTENT
                );

        params.setMargins(0, 8, 0, 8);

        if (user) {
            params.gravity = Gravity.END;
            messageView.setBackgroundColor(Color.rgb(35, 75, 110));
        } else {
            params.gravity = Gravity.START;
            messageView.setBackgroundColor(Color.rgb(24, 33, 43));
        }

        chatContainer.addView(messageView, params);
    }
}
