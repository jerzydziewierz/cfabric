# Demo 3 Sequence Diagram

```mermaid
sequenceDiagram
    participant UI as UserInterface
    participant B as Broker
    participant DP as DataProcessor
    participant L as Logger

    Note over UI,L: Application Start

    UI->>B: publish(SourceMessage)
    B->>DP: SourceMessage
    B->>L: SourceMessage
    B->>UI: SourceMessage

    DP->>B: publish(ProcessedMessage)
    B->>L: ProcessedMessage
    B->>UI: ProcessedMessage

    Note over UI,L: User Input Simulation

    UI->>B: publish(SourceMessage)
    B->>DP: SourceMessage
    B->>L: SourceMessage
    B->>UI: SourceMessage

    DP->>B: publish(ProcessedMessage)
    B->>L: ProcessedMessage
    B->>UI: ProcessedMessage

    Note over UI,L: Application Stop

    UI->>B: publish(StopSignal)
    B->>DP: StopSignal
    B->>L: StopSignal
    B->>UI: StopSignal
```

This sequence diagram shows the flow of messages between the UserInterface (UI), Broker (B), DataProcessor (DP), and Logger (L) modules over time. It includes the main message types (SourceMessage, ProcessedMessage, and StopSignal) and shows how they are published and distributed through the Broker.
