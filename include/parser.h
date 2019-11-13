#include <ArduinoJson.h>

class Parser {
    private:
        StaticJsonDocument<300> jsonOut;
        uint8_t position, cursor;
        char init, end, delimiter;
        float output[9], temp_output[9], change[9];
        char buffer[20];
        String sensors[9] = {"0","1","2","3","4",
        "5","6","7", "8"};
    public:
        Parser(char init, char delimiter, char end, int size);
        ~Parser(){};
        bool run(Stream &uart);
        float readData(int pos);
        String returnJson(void);
        bool alert(int pos, float threshold);
};

Parser::Parser(char init, char delimiter, char end, int size){
    this->init = init;
    this->delimiter = delimiter;
    this->end = end;
}

bool Parser::run(Stream &uart){
    if (uart.available()){
        char newByte = uart.read();
        Serial.print(newByte);
        if(newByte == init){
            position=0;
            cursor=0;
            return false;
        }
        else if (newByte == end){
            for(int i=0; i<9; i++){
                change[i]=temp_output[i]-output[i];      
                output[i]=temp_output[i];
            }
            return true;
        }
        else if(newByte == delimiter){
            if(position>0){
                char temp_buffer[cursor];
                for(int i = 0; i < cursor; i++){
                    temp_buffer[i] = buffer[i];
                }
                temp_output[position-1] = atof(temp_buffer);
            }
            cursor=0;
            position++;
            return false;
        }
        else {
            buffer[cursor] = newByte;
            cursor++;
            return false;
        }       
    } 
    return false;
};

float Parser::readData(int pos){
    return this->output[pos];
}

String Parser::returnJson(void){
    String out;
    StaticJsonDocument<300> jsonOut;
    
    for (int i=0; i<9; i++){
        jsonOut[sensors[i]] = this->readData(i);        
    }
    serializeJson(jsonOut, out);
    return out;
}

bool Parser::alert(int pos, float threshold){
    if(fabs(change[pos])>threshold){
        return true;
    }else{
        return false;
    }
}