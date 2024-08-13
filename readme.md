## Verlet Collision Simulation using R-trees

### Breif
Simulation of balls colliding using ```boost``` and ```SFML```.Collision simulation runs in a single thread thus allowing us to replicate images through falling balls. R-trees have been used to make collisions efficient. The ```boost/geometry``` library contains the used R-tree implementaion. 

### Instructions
- You have to include `boost_1_82_0`. Just download from [here](https://archives.boost.io/release/1.82.0/source/boost_1_82_0.zip) and extract to project root directory.The directory structure should look like `<root>\boost_1_82_0\boost_1_82_0\...`.
- Open the repo as a Visual Studio 2022 project.
- `SFML` should already be included when the repo is cloned.
- Change the `FILEPATH` macro to any image(ideally square) to render it using the falling balls
