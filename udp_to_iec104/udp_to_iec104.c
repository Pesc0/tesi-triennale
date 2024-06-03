
#include <arpa/inet.h>
#include <iec104api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT 5005
#define BUF_SIZE 65535


bool connected_to_client = false;


void printerror(const char *fnName, tErrorCode errCode, tErrorValue errValue) {
  // Error code
  struct sIEC104ErrorCode sIEC104ErrorCodeDes = {.iErrorCode = errCode};
  IEC104ErrorCodeString(&sIEC104ErrorCodeDes);

  // ADDITIONAL error code
  struct sIEC104ErrorValue sIEC104ErrorValueDes = {.iErrorValue = errValue};
  IEC104ErrorValueString(&sIEC104ErrorValueDes);

  printf("%s failed:  [%d : %s], [%d : %s] \n", fnName, errCode,
         sIEC104ErrorCodeDes.LongDes, errValue, sIEC104ErrorValueDes.LongDes);
}

Integer16 cbClientstatus(Unsigned16 u16ObjectId, struct sIEC104DataAttributeID *ptDataID, enum eStatus *peSat, tErrorValue *ptErrorValue)
{
  connected_to_client = !(*peSat  ==  NOT_CONNECTED);
  return EC_NONE;
}

// Com'è possibile che free possa fallire????
void free_iec104_client(IEC104Object client) {
  tErrorCode errCode = EC_NONE;
  tErrorValue errValue = EV_NONE;

  errCode = IEC104Free(client, &errValue);
  if (errCode != EC_NONE) {
    printerror("IEC104Free()", errCode, errValue);
  }
  
  client = NULL;
}

// Return client or NULL
IEC104Object init_iec104_client() {

  // Check library version against the library header file
  if (strcmp((char *)IEC104GetLibraryVersion(), IEC104_VERSION) != 0) {
    printf("\r\n Error: Version Number Mismatch");
    printf("\r\n Library Version is  : %s", IEC104GetLibraryVersion());
    printf("\r\n The Version used is : %s", IEC104_VERSION);
    printf("\r\n");
    return (0);
  }

  // IEC104 Client object callback paramters
  struct sIEC104Parameters sParameters = {
      .eAppFlag = APP_CLIENT, // This is a IEC104 CLIENT
      .u32Options = 0,
      .u16ObjectId = 1, // Client ID which used in callbacks to identify the
                        // iec 104 client object
      .ptClientStatusCallback = cbClientstatus,  // client connection status Callback
  };

  tErrorCode errCode = EC_NONE;
  tErrorValue errValue = EV_NONE;

  IEC104Object client = IEC104Create(&sParameters, &errCode, &errValue);
  if (client == NULL) {
    printerror("IEC104Create()", errCode, errValue);
    return NULL;
  }

  struct sClientConnectionParameters cli_1_config = {
      .ai8DestinationIPAddress = "127.0.0.1", // iec 104 server ip address
      .u16PortNumber = 2404,                  // iec 104 server port number
      .i16k = 12,
      .i16w = 8,
      .u8t0 = 30,
      .u8t1 = 15,
      .u8t2 = 10,
      .u16t3 = 20,
      .eState = DATA_MODE,
      .u8TotalNumberofStations = 1,
      .au16CommonAddress[0] = 1,
      .au16CommonAddress[1] = 0,
      .au16CommonAddress[2] = 0,
      .au16CommonAddress[3] = 0,
      .au16CommonAddress[4] = 0,
      .u8OriginatorAddress = 0,
      .u32CommandTimeout = 10000,
      .u32FileTransferTimeout = 50000,
      .bCommandResponseActtermUsed = TRUE,
      .bEnablefileftransfer = FALSE,
      .ai8FileTransferDirPath = "C:\\",
      .bUpdateCallbackCheckTimestamp = FALSE,
      .eCOTsize = COT_TWO_BYTE,

      // NULL because AutoGenIEC104DataObjects = TRUE
      .u16NoofObject = 0,
      .psIEC104Objects = NULL,
  };

  // Communication and protocol parameters
  struct sIEC104ConfigurationParameters sIEC104Config = {
      .sClientSet =
          {
              // client own IP Address , use 0.0.0.0 / network ip address for
              // binding socket
              .ai8SourceIPAddress = "0.0.0.0",
              .benabaleUTCtime = FALSE,
              .sDebug.u32DebugOptions = (DEBUG_OPTION_TX | DEBUG_OPTION_RX),
              .u16TotalNumberofConnection = 1,
              .bAutoGenIEC104DataObjects = TRUE,
              .psClientConParameters = &cli_1_config,
          },
  };

  errCode = IEC104LoadConfiguration(client, &sIEC104Config, &errValue);
  if (errCode != EC_NONE) {
    printerror("IEC104LoadConfiguration()", errCode, errValue);
    free_iec104_client(client);
    return NULL;
  }

  errCode = IEC104Start(client, &errValue);
  if (errCode != EC_NONE) {
    printerror("IEC104Start()", errCode, errValue);
    free_iec104_client(client);
    return NULL;
  }

  return client;
}

void stop_and_free_iec104_client(IEC104Object client) {
  if(client == NULL) { return; }

  tErrorCode errCode = EC_NONE;
  tErrorValue errValue = EV_NONE;

  errCode = IEC104Stop(client, &errValue);
  if (errCode != EC_NONE) {
    printerror("IEC104Stop()", errCode, errValue);
  }

  free_iec104_client(client);
}

void send_iec104_command(IEC104Object client, float f32data, Unsigned32 ioa) {

  // Command data identification parameters
  struct sIEC104DataAttributeID sWriteDAID = {
      .ai8IPAddress = "127.0.0.1",
      .u16PortNumber = 2404,
      .eTypeID = C_SE_TC_1,
      .u32IOA = ioa,
      .u16CommonAddress = 1,
  };

  time_t now;
  time(&now);
  struct tm *timeinfo = localtime(&now);

  // Command data value parameters
  struct sIEC104DataAttributeData sWriteValue = {
      .sTimeStamp =
          {
              .u16Year = timeinfo->tm_year + 1900,
              .u8Month = (Unsigned8)(timeinfo->tm_mon + 1),
              .u8Day = (Unsigned8)timeinfo->tm_mday,
              .u8Hour = (Unsigned8)timeinfo->tm_hour,
              .u8Minute = (Unsigned8)timeinfo->tm_min,
              .u8Seconds = (Unsigned8)timeinfo->tm_sec,
              .u16MilliSeconds = 0,
              .u16MicroSeconds = 0,
              .i8DSTTime = 0, // No Day light saving time
              .u8DayoftheWeek = 4,
          },

      .eDataType = FLOAT32_DATA,
      .eDataSize = FLOAT32_SIZE,
      .tQuality = GD,
      .pvData = &f32data,
  };

  // Command addional input parameters
  struct sIEC104CommandParameters sCommandParams = {0};

  tErrorCode errCode = EC_NONE;
  tErrorValue errValue = EV_NONE;

  errCode = IEC104Operate(client, &sWriteDAID, &sWriteValue, &sCommandParams,
                          &errValue);
  if (errCode != EC_NONE) {
    printerror("IEC104Operate()", errCode, errValue);
    return;
  }

  printf("Command send success\n");
}

int main(void) {

  IEC104Object myClient = init_iec104_client();
  if (myClient == NULL) {
    exit(EXIT_FAILURE);
  }

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("Could not create socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Socket bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  while (!connected_to_client) { usleep(1); }

  char buffer[BUF_SIZE];
  float value;
  while (1) {
    if (recv(sockfd, &buffer, BUF_SIZE,
             MSG_WAITALL ==
                 4)) { // Mi assicuro di aver ricevuto un float (4 bytes)
      memcpy(&value, buffer, sizeof(value));
      printf("Received: %f\n", value);
      send_iec104_command(myClient, value, 101);
    }
  }

  close(sockfd);
  stop_and_free_iec104_client(myClient);

  printf("\n");

  return (0);
}
