# Testare la comunicazione con gli esempi forniti da FreyrSCADA

```
cd FreyrSCADA_examples
make
```

Poi, in 2 terminali:
`./iec104clienttest`
`./iec104servertest`

- Lato Client:
```
s - send command
inserire IOA
inserire un numero
```

- Lato server:
Mostrerà la ricezione del numero


# Testare comunicazione UDP

- Terminale 1:
```
cd udp_receiver
python3 udp_receiver.py
```
Oppure per l'esempio in C:
```
gcc udp_receiver.c -o udp_receiver
./udp_receiver
```

- Terminale 2:
```
cd udp_sender
python3 udp_sender.py
```

# Testare il traduttore udp-iec104

- Terminale 1:
```
cd FreyrSCADA_examples
make
./iec104servertest 
```

- Terminale 2:
```
cd udp_to_iec104
make
./udp_to_iec104
```

- Terminale 3:
```
cd udp_sender
python3 udp_sender.py
```
