#include "canbus_wrapper.hpp"
#include "utils.hpp"

namespace CanOpenWrapper {

    class CANOpen : public CanBusBase::CanBusWrapper
    {
        private:

            int m_nNodeID;
            int m_nBaseIDReq;
            int m_nBaseIDResp;

            CANOpenUtils::canopen_frame m_coLastMsgSent;
            int m_nCounterCheck = 0;
            int m_nThreeshold = 0;

            void init();

            template<typename Base, typename T>
            inline bool instanceof(const T) { return std::is_base_of<Base, T>::value; }

        public:

            CANOpen(int nNodeID, int nSocketCan, int nBaseIDReq, int nBaseIDResp);
            void canBusListener(struct can_frame cfd);
            
            template <typename T>
            CANOpenUtils::canopen_frame download(uint16_t nIndex, uint8_t nSubIndex, T value, int nTimeOut=5)
            {
                CANOpenUtils::canopen_frame coFrame;
                CANOpenUtils::canopen_frame coRetFrame;

                if ((this->instanceof<double>(value)) || (this->instanceof<long long int>(value)))
                    throw CanNetworkBase::CANException(CanNetworkBase::MAX_LEN_EXCEEDED, "[ CANOPEN ]: Maximum payload length exceeded");

                coFrame = CANOpenUtils::getFrameFromData<T>(CANOpenUtils::DOWNLOAD_REQ, nIndex, nSubIndex, value);
                coFrame.canopen_id = this->m_nBaseIDReq + m_nNodeID;

                can_frame frame = CANOpenUtils::getCANBusFrameFromCANOpenFrame(coFrame);

                // Send the frame on the CANBus
                this->writeData(frame);

                this->m_coLastMsgSent = frame;

            }
    };

}