# Demo 3 Sequence Diagram

```mermaid
sequenceDiagram

    participant M as Main
    
    %% participant L as Logger
    %% participant B as Broker
    %% participant UI as UserInterface
    %% participant DP as DataProcessor

    Note over M: Application Start
    activate M
    create participant B as Broker
    M->>B: broker = create Broker 
    create participant DP as DataProcessor
    M->>DP: create DataProcessor(broker)
    create participant UI as UserInterface
    M->>UI: create UserInterface(broker)
    create participant L as Logger
    M->>L: create Logger(broker)
    alt user-driven
        Note over M,L: User driven
        loop user-input
            
            activate UI
            note over UI: interact with user
            UI->>B: publish(SourceMessage)
            deactivate UI
            activate B
            note over B: distribute SourceMessage
            B->>DP: SourceMessage
            activate DP
            note over DP: execute processing
            B->>L: SourceMessage
            B->>UI: SourceMessage
            deactivate B
            activate UI
            note over UI: display feedback<br>the SourceMessage has been sent for processing...
            deactivate UI
            

            DP->>B: publish(processedResult)
            deactivate DP

            activate B
            note over B: distribute processedResult
            
            B->>L: ProcessedMessage
            B->>UI: ProcessedMessage
            deactivate B
            activate UI
            note over UI: display results of processing    
            deactivate UI    
        end
    else batch-process
        Note over M,L: Batch Processing without display
        loop batch-process
            M->>B: publish(SourceMessage) from data store
            activate B
            note over B: distribute SourceMessage
            B->>DP: SourceMessage 
            deactivate B 
            activate DP
            note over DP: execute processing            
            DP->>B: publish(processedResult)
            deactivate DP
            activate B
            note over B: distribute processedResult
            B->>L: ProcessedMessage            
            deactivate B
        end
    end
    
    Note over M,L: Application Stop
    alt app-stop
        M->>B: publish(StopSignal)
        activate B
    else user-stop
        UI->>B: publish(StopSignal)
    end
    activate B
    note over B: distribute StopSignal
    destroy DP
    B->>DP: StopSignal    
    destroy UI
    B->>UI: StopSignal
    
    destroy L
    B->>L: StopSignal
    deactivate B     
    note right of M: Broker is destroyed last<br/>after all referencing modules are gone<br> and itself goes out of scope
    destroy B
    M->>B: Nothing references broker
    deactivate M
        
```

This sequence diagram now shows the flow of messages between Main (M), UserInterface (UI), Broker (B), DataProcessor (DP), and Logger (L) modules over time, accurately reflecting the structure in demo3.cpp.
