```mermaid
classDiagram
    Atic <-- First
    First <-- Ground
    Ground <-- Basement
    Basement <-- Cavern
    Ground <-- Cavern

    class Cavern {
        toBasement(425 to 321)
        toGround(442 to 33)
    }