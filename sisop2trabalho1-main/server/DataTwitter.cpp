 typedef struct __notification{ 
    uint32_t id;         //Identificador da notificação (sugere-se um identificador único) 
    uint32_t timestamp;     //Timestamp da notificação 
    uint16_t length;       //Tamanho da mensagem 
    uint16_t pending;      //Quantidade de leitores pendentes 
    const char* _string;     //Mensagem 
  } notification; 
class DataTwitter{
    string profile;
    Vector<string> followers;
    Vector<string> receivedMessage;
    Vector<notification> pendingNotification;
}
 