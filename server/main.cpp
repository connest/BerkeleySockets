
#include "server.h"
#include "serverprocessorexercise.h"
int main()
{
    IServerProcessor* processor = new ServerProcessorExercise();

//    std::string str = "20 apples, 30       55 ban77anas, 15 peaches and 1 watermelon";
//    std::cout<<processor->process(std::move(str))<<std::endl;

    Server s(3456);
    s.init(processor);
    s.start();
}
