# Demo 3 Sequence Diagram

```mermaid
sequenceDiagram
    participant L as Logger
    participant B as Broker
    participant UI as UserInterface
    participant DP as DataProcessor
    participant M as Main
    

    Note over M,L: Application Start
    M->>B: create Broker
    M->>L: create Logger(broker)
    M->>DP: create DataProcessor(broker)
    M->>UI: create UserInterface(broker)

    Note over M,L: Simulate User Input
    M->>UI: getUserInput()
    UI->>B: publish(SourceMessage)
    B->>DP: SourceMessage
    B->>L: SourceMessage
    B->>UI: SourceMessage

    DP->>B: publish(ProcessedMessage)
    B->>L: ProcessedMessage
    B->>UI: ProcessedMessage

    Note over M,L: Simulate Batch Processing
    M->>DP: processAndPublish("Batch data")
    DP->>B: publish(ProcessedMessage)
    B->>L: ProcessedMessage
    B->>UI: ProcessedMessage

    Note over M,L: Application Stop
    M->>B: publish(StopSignal)
    B->>DP: StopSignal
    B->>L: StopSignal
    B->>UI: StopSignal
```

This sequence diagram now shows the flow of messages between Main (M), UserInterface (UI), Broker (B), DataProcessor (DP), and Logger (L) modules over time, accurately reflecting the structure in demo3.cpp.
