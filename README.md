# PCInventory

One of my university assignments using C++. Main function contains premade tests provided by our university. Translated description is as follows:

The task is to design and implement a set of classes that will simulate the evidence of computer equipment of the company. Specifically, we will store information about CNetworks , computers ( CComputer ), their processors ( CCPU ), memories ( CMemory ) and disks ( CDisk ).

The task is focused on the design of classes where inheritance, polymorphism and abstract methods will be used. If these PPE are used rationally, the implementation is not too long. Conversely, if you make a wrong design, the code will be repeated and the implementation file will be large. Try to identify the base class and appropriately derive subclasses from it.

Classes and their interfaces:

CNetwork
represents the network. Its interface shall contain:
constructor with network name,
destructor, copy constructor and operator = (if custom implementation is needed)
AddComputer method to add another computer to the network
The FindComputer method , which returns a link to the found computer of the specified name or an invalid pointer if it is not found
an output operator that displays a tree of computers and components, as in the example. Computers are listed in the order of adding.

CComputer
represents a computer. Its interface shall contain:
a constructor with the computer name parameter
destructor, copy constructor and operator = (if custom implementation is needed)
AddComponent method that adds another computer component
AddAddress method to add another computer address (string)
an output operator that displays the assigned addresses and computer components, as in the example. The list contains first the addresses (in the order of entry) followed by the components (in the order of adding).

CCPU
represents the CPU. Its interface shall contain:
constructor with the number of cores (integer) and frequency (integer in MHz) parameters,
destructor, copy constructor and = operator (if custom implementation is needed).

CMemory
represents RAM. Its interface shall contain:
constructor with memory size parameter (integer in MiB),
destructor, copy constructor and = operator (if custom implementation is needed).

CDisk
represents the repository. Its interface shall contain:
a constructor with disk type parameters (symbolic constant SSD or MAGNETIC declared in class) and disk size (integer in GiB),
destructor, copy constructor and operator = (if custom implementation is needed)
AddPartition method , which adds partition information. The method will have two parameters - plot size in GiB and its identification (string). The parcel listing is in the order of entry.
