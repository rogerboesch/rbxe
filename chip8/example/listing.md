| Adress   | Opcode  | Mnemonic       | Comment
|----------|---------|----------------|--------------------------------------
| 0x200    | 00E0    | CLS            | Clear screen
| 0x202    | 600F    | LD V0, 0x0F    | Player X-position = 15
| 0x204    | 6101    | LD V1, 0x01    | Player Y-position = 1
| 0x206    | 620F    | LD V2, 0x0F    | Block X-position = 15
| 0x208    | 6300    | LD V3, 0x00    | Block Y-position = 0
| **Main loop** |
| 0x20A    | D013    | DRW V0, V1, 3  | Player draw (1 Zeile)
| 0x20C    | D231    | DRW V2, V3, 1  | Block draw (1 Zeile)
| **Check input** |
| 0x20E    | E49E    | SKP V4         | If key in V4 pressed (key 4 = left)
| 0x210    | 7002    | ADD V0, 0x02   |   → Player to right
| 0x212    | E69E    | SKP V6         | If key in V6 pressed (key 6 = right)
| 0x214    | 7002    | ADD V0, 0x02   |   → Player to right
| **Move block down** | 
| 0x216    | 7301    | ADD V3, 0x01   | Block Y += 1
| **Collission test** |
| 0x218    | 4031    | SE V0, V1      | If V0 == V1 (X equal)
| 0x21A    | 5002    | SE V0, V2      | Und V0 == V2 (Y equal)
| 0x21C    | 122A    | JP 0x22A       | → Game Over
| **Simple pause/delay** |
| 0x21E    | 6005    | LD V0, 0x05    | Timer start with 5
| 0x220    | 8000    | LD V0, V0      | NOP
| 0x222    | 7001    | ADD V0, 0x01   | V0 += 1
| 0x224    | 300A    | SE V0, 0x0A    | If V0 == 10
| 0x226    | 1220    | JP 0x220       | → still wait
| **Back to mainloop** |
| 0x228    | 120A    | JP 0x20A       | Back to main loop
| **Game over state** |
| 0x22A    | 00E0    | CLS            | Clear screen
| 0x22C    | 122C    | JP 0x22C       | Endless loop (Game Over)
