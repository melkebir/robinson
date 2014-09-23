git clone https://seminaro@bitbucket.org/mohammed_el_kebir/robinson.git
cd robinson
mkdir build

cd build
cmake ..
make

% help
./robinson -h 

% generate Robinsonian matrix

./robinson -g 100 -L 5000

./robinson -g 100 -L 5000 > /tmp/matrix.txt

./robinson -s /tmp/matrix.txt

for i in {1..1000};do echo -n "$i ";./robinson -g 100 -L 5000 | ./robinson -s -;done
