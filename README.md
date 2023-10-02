# Procedural Narrative Generation

In coordination with my startup Vita Publishing, this is an in-progress open-source project (built in Unreal Engine 4.27) to investigate procedurally generated narratives systematically created from inputted event nodes and character attributes. The specification of the project is to implement dynamic character generation with social and romantic relationships that all play a factor in the final generated narrative.

Other approaches taken to the question of procedural generation pose using pathfinding or an A-Star-esque algorithm to methodically work forward from the start of the narrative to the end. This project proposes an alternative methodology.

Instead of trying to maintain logical coherence through analyzing individual events themselves, we will implement a reverse genetic algorithm where the initial parent is the conclusion event randomly selected at the beginning. We will work our way back up to generate an introduction event. This introduction event will then be selected by determining which of our introductions most logically fits the needed character states.

The heuristic algorithm then will rely almost primarily on the state or attitudes of the current characters. This logically makes more sense than pure event heuristics because it ensures coherence with the actions that characters take. We also then limit the potential metadata required.

In its current form, the project is only capable of generating text and is reliant on a steady input of possible dynamic event nodes to choose from. Future revisions and commits will address both issues with AI voice acting and implementing a GPT model in Unreal Engine for better and more efficient event generation.

More Information on Algorithm: https://docs.google.com/document/d/1Ja2FXIdiodFuYLFFwcsoCOsVz8CaFRFQ3FtEyPSMjbQ/edit
