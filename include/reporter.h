class Reporter {
    public:
        Reporter(int id){
            this->setid(id);
        }
        void report(Parser &inp);
        void setid(int id);
    private:
        int id;
};

void Reporter::report(Parser &inp){
    StaticJsonDocument<300> output;

    String out = inp.returnJson();

    LoRa.beginPacket();
    LoRa.print("<");
    LoRa.print(this->id);
    LoRa.print(">");
    LoRa.print(out);
    LoRa.endPacket();

}

void Reporter::setid(int id){
    this->id = id;
} 