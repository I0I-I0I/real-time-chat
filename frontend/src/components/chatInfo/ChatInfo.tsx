import cls from "@/utils/cls"
import styles from "./ChatInfo.module.css"

import { Typography } from "@/components/UI"
import { ReactSVG } from "react-svg"

interface ChatInfoProps {
    className?: string
}

export const ChatInfo = ({
    className = ""
}: ChatInfoProps) => (
    <div className={cls(styles.chat_info, className)}>
        <ReactSVG
            className={styles.icon}
            src="/account.svg"
        />
        <Typography tag="h2" variant="title-4">
            Name
        </Typography>
        <Typography tag="span" variant="text_small">
            last seen
        </Typography>
    </div>
)
