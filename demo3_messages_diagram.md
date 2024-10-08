# Demo 3 Messages and Module Interactions

```mermaid
graph TD
    SM[SourceMessage]
    PM[ProcessedMessage]
    LOM[LogOnlyMessage]
    SS[StopSignal]

    UI[UserInterface]
    DP[DataProcessor]
    L[Logger]

    UI -->|Publishes| SM
    UI -->|Publishes| SS

    SM -->|Subscribed by| UI
    SM -->|Subscribed by| DP
    SM -->|Subscribed by| L

    DP -->|Publishes| PM

    PM -->|Subscribed by| UI
    PM -->|Subscribed by| L

    LOM -->|Subscribed by| L

    SS -->|Subscribed by| UI
    SS -->|Subscribed by| DP
    SS -->|Subscribed by| L
```

This diagram shows the different message types (SourceMessage, ProcessedMessage, LogOnlyMessage, and StopSignal) and which modules can publish or subscribe to each message type.
