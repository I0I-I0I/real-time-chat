import { IChat, IMessage } from "@/types"
import { create } from "zustand"

interface ChatStore {
    data: IChat | null,
    messages: IMessage[],
    setCurrentChat: (chat: IChat) => void
    setMessages: (messages: IMessage[]) => void
    addMessage: (message: IMessage) => void
}

export const useChatStore = create<ChatStore>()((set) => ({
    data: null,
    messages: [],
    setCurrentChat: (chat: IChat) => set({ data: chat }),
    setMessages: (messages: IMessage[]) => set({ messages }),
    addMessage: (message: IMessage) => set((state) => ({
        messages: [...(state.messages || []), message]
    }))
}))
