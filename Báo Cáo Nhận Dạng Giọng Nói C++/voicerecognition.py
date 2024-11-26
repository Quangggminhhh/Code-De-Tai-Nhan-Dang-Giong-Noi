import speech_recognition
import os

recognizer = speech_recognition.Recognizer()

def split_text(text, max_length=18):
    # Chia văn bản thành các phần nhỏ có độ dài tối đa max_length
    return [text[i:i+max_length] for i in range(0, len(text), max_length)]

with open("123.txt", "w") as f:
    while True:
        if os.path.exists("stop_python.txt"):
            break
        try:
            with speech_recognition.Microphone() as mic:
                recognizer.adjust_for_ambient_noise(mic, duration=0.2)
                print("Listening...")
                audio = recognizer.listen(mic)

            text = recognizer.recognize_google(audio)
            text = text.lower()
            print(f"Recognized: {text}")

            if text == "stop":
                break 

            # Chia văn bản thành các dòng nhỏ hơn nếu vượt quá 18 ký tự
            lines = split_text(text, 34)
            for line in lines:
                f.write(line + "\n")

        except speech_recognition.UnknownValueError:
            print("Could not understand audio")
        except speech_recognition.RequestError as e:
            print(f"Could not request results; {e}")
        except Exception as e:
            print(f"An error occurred: {e}")