#ifndef TENSORFLOW_LITE_MODEL_H_
#define TENSORFLOW_LITE_MODEL_H_

// Classes that can be detected by the neural network
extern const char* available_classes[];
extern const int available_classes_num;

const char* available_classes[] = {"circle", "up_down", "left_right", "idle"}; // <--- CHANGE THIS LINE
const int available_classes_num = sizeof(available_classes) / sizeof(available_classes[0]);


// Pre-trained netural network
extern const unsigned char gesture_model[];
//extern const int gesture_model_len;

#endif /* TENSORFLOW_LITE_MODEL_H_ */