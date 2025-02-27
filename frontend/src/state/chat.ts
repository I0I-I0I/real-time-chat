import { IChat, IMessage } from "@/types"
import { create } from "zustand"

interface ChatStore {
    data: IChat | null,
    messages: IMessage[] | null,
    setCurrentChat: (chat: IChat) => void
    addMessage: (message: IMessage) => void
}

export const useChatStore = create<ChatStore>()((set) => ({
    data: null,
    messages: [],
    setCurrentChat: (chat: IChat) => set({ data: chat }),
    addMessage: (message: IMessage) => set((state) => ({
        messages: [...(state.messages || []), message]
    }))
}))
