# Demo 3 Modules and Message Flow

```mermaid
graph TD
    UI[UserInterface]
    DP[DataProcessor]
    L[Logger]
    B[Broker]

    UI -->|SourceMessage| B
    B -->|SourceMessage| DP
    B -->|SourceMessage| L
    B -->|SourceMessage| UI

    DP -->|ProcessedMessage| B
    B -->|ProcessedMessage| L
    B -->|ProcessedMessage| UI

    UI -->|StopSignal| B
    B -->|StopSignal| DP
    B -->|StopSignal| L
    B -->|StopSignal| UI

    UI -->|LogOnlyMessage| B
    B -->|LogOnlyMessage| L
```

This diagram shows the main modules (UserInterface, DataProcessor, and Logger) and how they interact through the Broker. The arrows represent the flow of different message types between the modules.
