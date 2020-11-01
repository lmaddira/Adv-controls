# Adv-controls

Install osqp-python using following instructions
# pip
`pip install osqp`

# Anaconda
`conda install -c conda-forge osqp`

<!-- you might need to install GCC Compiler , CMake and Visual studio C++ compiler  -->

Now you are ready to build OSQP python interface from sources. Run the following in your terminal
```
git clone --recurse-submodules https://github.com/oxfordcontrol/osqp-python
cd osqp-python
python setup.py install

```
Ignore other files as we are developing code in python

Please look into src->mpc_lab.py
this is the basic mpc horizon tracking example we had in matlab (provided by professor)



