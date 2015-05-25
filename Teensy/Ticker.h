/*
 * simple container class to store state and effect 
 */

#ifndef Ticker_h
#define Ticker_h

class Ticker {
    private:
        int m_state;
        String m_color;
        bool m_bounce;
        int m_direction;
        String m_text;

    public:
        Ticker(int iState=0, String sColor="FFFFFF", bool bBounce=false, int iDirection=1, String sText="") { 
            m_state = iState;
            m_color = sColor;
            m_bounce = bBounce;
            m_direction = iDirection;
            m_text = sText;
        }

        int state() { return m_state; }
        String color() { return m_color; }
        int red() { return m_color.substring(0,2).toInt(); }
        int green() { return m_color.substring(2,2).toInt(); }
        int blue() { return m_color.substring(4,2).toInt(); }
        bool bounce() { return m_bounce; }
        int direction() { return m_direction; }
        String text() { return m_text; }
        
        void setState(int iState) { m_state = iState; }
        void setColor(String sColor) { m_color = sColor; }
        void setBounce(bool bBounce) { m_bounce = bBounce; }
        void setDirection(int iDirection) { m_direction = iDirection; }
        void setText(String sText) { m_text = sText; }
};

#endif // Ticker_h
