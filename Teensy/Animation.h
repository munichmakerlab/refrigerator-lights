/*
 * simple container class to store state and effect 
 */

#ifndef Animation_h
#define Animation_h

class Animation {
    private:
        int m_state;
        int m_effect;

    public:
        Animation(int iState=0, int iEffect=1) { 
            m_state = iState; 
            m_effect = iEffect; 
        }

        int state() { return m_state; }
        int effect() { return m_effect; }
        void setState(int iState) { m_state = iState; }
        void setEffect(int iEffect) { m_effect = iEffect; }
};

#endif // Animation_h
