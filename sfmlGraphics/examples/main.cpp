#include <Viewer.hpp>
#include <log.hpp>

#include <sstream>

void initialize_practical_02_scene( Viewer& viewer );
void initialize_practical_03_scene( Viewer& viewer );
void initialize_practical_04_scene( Viewer& viewer );
void initialize_practical_05_scene( Viewer& viewer );
void initialize_practical_06_scene( Viewer& viewer );
void initialize_practical_07_scene( Viewer& viewer );
void initialize_practical_07_triangle( Viewer& viewer );

int main( int argc, char* argv[] )
{
    Viewer viewer(1280,720);

    unsigned int scene_to_load = 2;

    for( int i = 1; i < argc; ++ i )
    {
        std::istringstream tokenizer( std::string( argv[i] ) );
        tokenizer >> scene_to_load;
        if( tokenizer.fail() )
        {
            LOG( fatal, "unknown argument " << argv[i] << ". Please launch without argument or the number of the practical you want to load");
            exit( EXIT_FAILURE );
        }
    }

    if( scene_to_load == 2 )
    {
        initialize_practical_02_scene( viewer );
    }
    else if( scene_to_load == 3 )
    {
        initialize_practical_03_scene( viewer );
    }
    else if( scene_to_load == 4 )
    {
        initialize_practical_04_scene( viewer );
    }
    else if( scene_to_load == 5 )
    {
        initialize_practical_05_scene( viewer );
    }
    else if( scene_to_load == 6 )
    {
        initialize_practical_06_scene( viewer );
    }
    else if( scene_to_load == 7 )
    {
        initialize_practical_07_scene( viewer );
    }
    else
    {
        LOG( fatal, "unknown practical number " << scene_to_load );
        exit( EXIT_FAILURE );
    }

    while( viewer.isRunning() )
    {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}
