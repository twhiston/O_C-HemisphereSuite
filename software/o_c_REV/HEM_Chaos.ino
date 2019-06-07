// Copyright (c) 2018, Tom Whiston
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// From maxpatch
// should be able to set params with input. which make the most sense?
// todo - other algos and choose on screen

#define CHAOS_AVAILABLE_ALGO 1

class ChaosAlgo {

    public:

        // Called in the hemisphere app Start() function
        void start(){};

        void cvIn(int a, int b){};

        // Called when clocked in input 0
        void run(){};

        int getX(){
            return this->x;
        }
        int getY(){
            return this->y;
        }

    protected:

        float x;
        float y;
        float z;
};


class ChaoSimplified : public ChaosAlgo {

public:

    void start(){
        this->param_p = 10.;
        this->param_b = 8.0/3.0;
        this->param_r = 30.5;
        this->param_TS = 0.0003;
    };

    void run(){
	        //calculate change
	        float dx = this->param_p * (this->y - this->x);
	        float dy = (this->x * (this->param_r - this->y)) - this->y;
	        float dz = (this->x * this->y) - (this->param_b * this->y);
	        //update histories
	        this->x = this->x + dx * this->param_TS;
	        this->y = this->y + dy * this->param_TS;
	        this->y = this->y + dz * this->param_TS;
    };

    private:
        // in max params can be set from outside the dsp code
        float param_p;
        float param_b;
        float param_r;
        float param_TS;
};


class Chaos : public HemisphereApplet {
public:

    Chaos() : algos{ChaoSimplified{}}{}

    const char* applet_name() { // Maximum 10 characters
        return "Chaos";
    }

	/* Run when the Applet is selected */
    void Start() {
        this->selectedAlgo = 0;
        for (uint8_t i = 0; i < CHAOS_AVAILABLE_ALGO; i++)
        {
            this->algos[i].start();
        }
    }

	/* Run during the interrupt service routine, 16667 times per second */
    void Controller() {

        if (Clock(0)) {	
	        this->algos[selectedAlgo].run(In(0), In(1));
            Out(0, this->algos[selectedAlgo].getX());
            Out(1, this->algos[selectedAlgo].getY());
        }
    }

	/* Draw the screen */
    void View() {
        gfxHeader(applet_name());
        gfxSkyline();
        // Add other view code as private methods
    }

	/* Called when the encoder button for this hemisphere is pressed */
    void OnButtonPress() {
    }

	/* Called when the encoder for this hemisphere is rotated
	 * direction 1 is clockwise
	 * direction -1 is counterclockwise
	 */
    void OnEncoderMove(int direction) {
    }
        
    /* Each applet may save up to 32 bits of data. When data is requested from
     * the manager, OnDataRequest() packs it up (see HemisphereApplet::Pack()) and
     * returns it.
     */
    uint32_t OnDataRequest() {
        uint32_t data = 0;
        // example: pack property_name at bit 0, with size of 8 bits
        // Pack(data, PackLocation {0,8}, property_name); 
        return data;
    }

    /* When the applet is restored (from power-down state, etc.), the manager may
     * send data to the applet via OnDataReceive(). The applet should take the data
     * and unpack it (see HemisphereApplet::Unpack()) into zero or more of the applet's
     * properties.
     */
    void OnDataReceive(uint32_t data) {
        // example: unpack value at bit 0 with size of 8 bits to property_name
        // property_name = Unpack(data, PackLocation {0,8}); 
    }

protected:
    /* Set help text. Each help section can have up to 18 characters. Be concise! */
    void SetHelp() {
        //                               "------------------" <-- Size Guide
        help[HEMISPHERE_HELP_DIGITALS] = "Digital in help";
        help[HEMISPHERE_HELP_CVS]      = "CV in help";
        help[HEMISPHERE_HELP_OUTS]     = "Out help";
        help[HEMISPHERE_HELP_ENCODER]  = "123456789012345678";
        //                               "------------------" <-- Size Guide
    }
    
private:

    // TODO should be an array of the base class so we can select one and run it in a clean way
    uint8_t selectedAlgo;
    ChaosAlgo algos[CHAOS_AVAILABLE_ALGO];

};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Chaos,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Chaos Chaos_instance[2];

void Chaos_Start(bool hemisphere) {Chaos_instance[hemisphere].BaseStart(hemisphere);}
void Chaos_Controller(bool hemisphere, bool forwarding) {Chaos_instance[hemisphere].BaseController(forwarding);}
void Chaos_View(bool hemisphere) {Chaos_instance[hemisphere].BaseView();}
void Chaos_OnButtonPress(bool hemisphere) {Chaos_instance[hemisphere].OnButtonPress();}
void Chaos_OnEncoderMove(bool hemisphere, int direction) {Chaos_instance[hemisphere].OnEncoderMove(direction);}
void Chaos_ToggleHelpScreen(bool hemisphere) {Chaos_instance[hemisphere].HelpScreen();}
uint32_t Chaos_OnDataRequest(bool hemisphere) {return Chaos_instance[hemisphere].OnDataRequest();}
void Chaos_OnDataReceive(bool hemisphere, uint32_t data) {Chaos_instance[hemisphere].OnDataReceive(data);}
